#version 450
#define VOX_DEFERRED_LAYOUT

#include "../src/engine/game/game_decl.h"
#include "../src/engine/gfx/descriptor/sets/descriptor_decl.h"
#include "../src/engine/vox_decl.h"

layout(location = 0) in uint inData;

layout(location = 0) out vec3 outUVW;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec3 outPosition;
#if ENABLE_SSAO
layout(location = 3) out vec3 outNormalView;
layout(location = 4) out vec3 outPositionView;
#endif

layout(push_constant) uniform Camera {
    mat4 view;
    mat4 proj;
} camera;

layout(set = PFD_SET, binding = 0) uniform GameData {
    vec4 dummy;
    uvec2 portionOffset;
    uvec2 renderOffset;
};

layout(set = WORLD_SET, binding = 2) uniform usampler2DArray ChunkData;
layout(set = WORLD_SET, binding = 3) uniform RenderData {
    uint renderAreaSide;
    uint fogDistance;
} renderData;

#define CORNER_A vec3(1.0, 0.0, 1.0)
#define CORNER_B vec3(1.0, 0.0, 0.0)
#define CORNER_C vec3(1.0, 1.0, 0.0)
#define CORNER_D vec3(1.0, 1.0, 1.0)
#define CORNER_E vec3(0.0, 0.0, 1.0)
#define CORNER_F vec3(0.0, 0.0, 0.0)
#define CORNER_G vec3(0.0, 1.0, 0.0)
#define CORNER_H vec3(0.0, 1.0, 1.0)

const vec3 CUBE_FACE[6][4] = {
    { CORNER_C, CORNER_D, CORNER_G, CORNER_H }, // Top
    { CORNER_A, CORNER_B, CORNER_E, CORNER_F }, // Bottom
    { CORNER_F, CORNER_G, CORNER_E, CORNER_H }, // Left
    { CORNER_A, CORNER_D, CORNER_B, CORNER_C }, // Right
    { CORNER_E, CORNER_H, CORNER_A, CORNER_D }, // Front
    { CORNER_B, CORNER_C, CORNER_F, CORNER_G }, // Back
};

const vec2 UVS[4] = {
    { 1.0, 1.0 },
    { 1.0, 0.0 },
    { 0.0, 1.0 },
    { 0.0, 0.0 },
};

const vec3 NORMALS[6] = {
    {  0.0,  1.0,  0.0 },
    {  0.0, -1.0,  0.0 },
    { -1.0,  0.0,  0.0 },
    {  1.0,  0.0,  0.0 },
    {  0.0,  0.0,  1.0 },
    {  0.0,  0.0, -1.0 },
};

const uint TEXTURE_INDEX[8][6] = {
    { 5, 5, 5, 5, 5, 5 }, // Air
    { 2, 0, 1, 1, 1, 1 }, // Grass
    { 0, 0, 0, 0, 0, 0 }, // Dirt
    { 3, 3, 3, 3, 3, 3 }, // Stone
    { 5, 5, 5, 5, 5, 5 }, // Snow
    { 4, 4, 4, 4, 4, 4 }, // Sand
    { 5, 5, 5, 5, 5, 5 }, // Wood
    { 5, 5, 5, 5, 5, 5 }, // Water
};

struct InstanceData {
    vec3 chunkOffset;   // 16 bits
    vec3 blockPos;      // 9 bits
    uint face;          // 3 bits

    uint textureIndex;  // deduced
};

uint getMaterial(in vec3 blockPosLocal, in uint chunk) {
    const vec2 dim = textureSize(ChunkData, 0).xy;

    vec2 remapPos = vec2(
        blockPosLocal.y + (blockPosLocal.x * 16),
        blockPosLocal.z);

    vec3 uvw = vec3(remapPos / dim, float(chunk));

    return texture(ChunkData, uvw).r;
}

InstanceData unpackData(in uint inputData) {
    InstanceData instanceData;

    uint blockId = inputData & 0xFFff;
    uint chunkId = inputData >> 16 & 0x3Ff;

    instanceData.chunkOffset = CHUNK_SIZE * vec3(
        chunkId % renderData.renderAreaSide,
        0.0,
        chunkId / renderData.renderAreaSide);

    instanceData.blockPos = vec3(
        float((blockId >> 12) & 0xF),
        float((blockId >> 4) & 0xFF),
        float(blockId & 0xF));

    uint material = getMaterial(instanceData.blockPos, chunkId);

    instanceData.face = (inputData >> (16+10)) & 0x7;
    instanceData.textureIndex = TEXTURE_INDEX[material][instanceData.face];

    return instanceData;
}

void main() {
    const InstanceData instanceData = unpackData(inData);
    const vec4 worldPos = vec4(CUBE_FACE[instanceData.face][gl_VertexIndex] + instanceData.chunkOffset + instanceData.blockPos, 1.0);

    outUVW = vec3(UVS[gl_VertexIndex], instanceData.textureIndex);
    outNormal = NORMALS[instanceData.face];
    outPosition = worldPos.xyz;

#if ENABLE_SSAO
    const mat3 invView = transpose(inverse(mat3(camera.view)));
    outNormalView = invView * outNormal;
    outPositionView = (camera.view * worldPos).xyz;
#endif

    gl_Position = camera.proj * camera.view * worldPos;
}