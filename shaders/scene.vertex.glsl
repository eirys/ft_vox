#version 450

layout(location = 0) out vec3 outUV;

layout(set = 0, binding = 0) uniform ViewProjData {
    mat4 inner;
} viewProj;

layout(set = 1, binding = 0) uniform usampler2DArray heightmap;

const vec2 vertexPos[4] = {
    { 0.0, 0.0 },
    { 1.0, 0.0 },
    { 0.0, 1.0 },
    { 1.0, 1.0 },
};

#define CHUNK_AREA 256;

void main() {
    uint chunk = gl_InstanceIndex / CHUNK_AREA;
    uint block = (gl_InstanceIndex - chunk) % CHUNK_AREA;

    vec2 blockUV = vec2(block % 16, block / 16);
    float height = texture(heightmap, vec3(blockUV / textureSize(heightmap, 0).xy, chunk)).r;

    vec2 vertex = vertexPos[gl_VertexIndex] + blockUV;
    vec4 worldPos = vec4(vertex.x, height, vertex.y, 1.0);

    outUV = vec3(vertexPos[gl_VertexIndex], 3);
    gl_Position = viewProj.inner * worldPos;
}