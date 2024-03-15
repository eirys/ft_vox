#version 450

layout(set = 0, binding = 0) uniform ViewProjData {
    mat4 inner;
} viewProj;

layout(set = 1, binding = 0) uniform WorldData {
    uint heights[1*1][16*16];
} world;

const vec2 quadpos[4] = {
    { 0.0, 0.0 },
    { 1.0, 0.0 },
    { 0.0, 1.0 },
    { 1.0, 1.0 },
};

void main() {
    const uint chunkSize = 16;
    const uint worldSize = 1;

    uint localIndex = gl_InstanceIndex % (chunkSize * chunkSize);
    uint chunkIndex = gl_InstanceIndex / (chunkSize * chunkSize);

    uvec2 localPos = uvec2(localIndex % chunkSize, localIndex / chunkSize);
    uvec2 chunkPos = uvec2(chunkIndex % worldSize, chunkIndex / worldSize);

    uvec2 worldPos2d = chunkPos * chunkSize + localPos;
    uint height = world.heights[chunkIndex][localIndex];

    vec2 posXZ = quadpos[gl_VertexIndex] + vec2(worldPos2d);
    vec3 worldPos = vec3(posXZ.x, float(height), posXZ.y);

    gl_Position = viewProj.inner * vec4(worldPos, 1.0);
}