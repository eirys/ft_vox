#version 450
#define VOX_SHADOW_LAYOUT

#include "../src/engine/game/game_decl.h"
#include "../src/engine/gfx/descriptor/sets/descriptor_decl.h"

layout(location = 0) in uint inData;

layout(set = PFD_SET, binding = 1) uniform Projector {
    mat4 viewProj;
} projector;
layout(set = WORLD_SET, binding = 3) uniform RenderData {
    uint worldSide;
    uint fogDistance;
    uvec2 portionOffset;
    uvec2 renderOffset;
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

struct InstanceData {
    vec3 chunkOffset;   // 16 bits
    vec3 blockPos;      // 9 bits
    uint face;          // 3 bits
};

InstanceData unpackData(in uint inputData) {
    InstanceData instanceData;

    uint blockId = inputData & 0xFFff;
    uint chunkId = inputData >> 16 & 0x3Ff;

    instanceData.chunkOffset = CHUNK_SIZE * vec3(
        (chunkId % renderData.worldSide),
        0.0,
        (chunkId / renderData.worldSide));

    instanceData.blockPos = vec3(
        float((blockId >> 12) & 0xF),
        float((blockId >> 4) & 0xFF),
        float(blockId & 0xF));

    instanceData.face = (inputData >> (16+10)) & 0x7;

    return instanceData;
}

void main() {
    const InstanceData instanceData = unpackData(inData);
    const vec3 worldPos =
        CUBE_FACE[instanceData.face][gl_VertexIndex] +
        instanceData.chunkOffset +
        instanceData.blockPos;

    gl_Position = projector.viewProj * vec4(worldPos, 1.0);
}