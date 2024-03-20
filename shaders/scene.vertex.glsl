#version 450

#include "../src/engine/game/game_decl.h"

layout(location = 0) out vec3 outUV;

layout(set = 0, binding = 0) uniform ViewProj {
    mat4 inner;
} viewProj;

layout(set = 0, binding = 1) uniform Time {
    float inner;
} time;

layout(set = 0, binding = 2) uniform Camera {
    float forward;
    float right;
    float up;
} camera;

layout(set = 1, binding = 0) uniform usampler2DArray heightmap;

const vec2 vertexPos[4] = {
    { 0.0, 0.0 },
    { 1.0, 0.0 },
    { 0.0, 1.0 },
    { 1.0, 1.0 },
};

void main() {
    uint chunk = gl_InstanceIndex / CHUNK_AREA;
    uint block = (gl_InstanceIndex - chunk) % CHUNK_AREA;

    vec2 chunkPos = vec2(chunk % WORLD_WIDTH, chunk / WORLD_DEPTH) * CHUNK_SIZE;
    vec2 blockPos = vec2(block % CHUNK_SIZE, block / CHUNK_SIZE);
    vec2 vertex = vertexPos[gl_VertexIndex] + blockPos + chunkPos;

    vec2 blockUV = blockPos / textureSize(heightmap, 0).xy;
    float height = texture(heightmap, vec3(blockUV, chunk)).r;

    vec3 worldPos = vec3(vertex.x, height, vertex.y);

    outUV = vec3(vertexPos[gl_VertexIndex], 3);
    gl_Position = viewProj.inner * vec4(worldPos, 1.0);
}