#version 450
#define VOX_SHADOW_LAYOUT

#include "../src/engine/game/game_decl.h"
#include "../src/engine/gfx/descriptor/sets/descriptor_decl.h"

layout(location = 0) in uint inData;

layout(set = PFD_SET, binding = 1) uniform Projector {
    mat4 viewProj;
} projector;

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
    vec3 chunkPos;  // 14 bits
    vec3 blockPos;  // 12 bits
    uint face;    // 3 bits
    uint textureIndex; // 3 bits
};

InstanceData unpackData(in uint inputData) {
    InstanceData instanceData;

    uint textureIndex = (inputData >> 29) & 0x7;
    uint face = (inputData >> 26) & 0x7;
    uint blockId = (inputData >> 14) & 0xFFF;
    uint chunkId = inputData & 0x3FFF;

    instanceData.chunkPos = CHUNK_SIZE * vec3(
        float((chunkId >> 9) & 0x1F),
        float((chunkId >> 5) & 0xF),
        float(chunkId & 0x1F)
    );

    instanceData.blockPos = vec3(
        float((blockId >> 8) & 0xF),
        float((blockId >> 4) & 0xF),
        float(blockId & 0xF)
    );

    instanceData.face = face;

    instanceData.textureIndex = textureIndex;

    return instanceData;
}

void main() {
    InstanceData instanceData = unpackData(inData);

    vec3 worldPos =
        CUBE_FACE[instanceData.face][gl_VertexIndex] +
        instanceData.chunkPos +
        instanceData.blockPos;

    gl_Position = projector.viewProj * vec4(worldPos, 1.0);
}