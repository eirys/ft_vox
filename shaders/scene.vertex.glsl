#version 450

#include "../src/engine/game/game_decl.h"

layout(location = 0) out vec2 outUV;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec3 outSunDir;

layout(set = 0, binding = 0) uniform ViewProj {
    mat4 view;
    mat4 proj;
} viewProj;

layout(set = 0, binding = 1) uniform GameData {
    vec2 sunPos;
} gameData;

layout(set = 1, binding = 0) uniform usampler2DArray heightmap;

const vec2 vertexPos[4] = {
    { 0.0, 0.0 },
    { 1.0, 0.0 },
    { 0.0, 1.0 },
    { 1.0, 1.0 },
};

vec3 getBlockPosition() {
    uint chunk = gl_InstanceIndex / CHUNK_AREA;
    uint block = (gl_InstanceIndex - chunk) % CHUNK_AREA;

    vec2 chunkPos = vec2(chunk % WORLD_WIDTH, chunk / WORLD_DEPTH) * CHUNK_SIZE;
    vec2 blockPos = vec2(block % CHUNK_SIZE, block / CHUNK_SIZE);
    vec2 vertex = vertexPos[gl_VertexIndex] + blockPos + chunkPos;

    vec2 blockUV = blockPos / textureSize(heightmap, 0).xy;
    float height = texture(heightmap, vec3(blockUV, chunk)).r;

    return vec3(vertex.x, height, vertex.y);
}

void main() {
    vec3 worldPos = getBlockPosition();

    outUV = vertexPos[gl_VertexIndex];
    outNormal = vec3(0.0, 1.0, 0.0);
    outSunDir = vec3(gameData.sunPos, 0.0);

    gl_Position = viewProj.proj * viewProj.view * vec4(worldPos, 1.0);
}