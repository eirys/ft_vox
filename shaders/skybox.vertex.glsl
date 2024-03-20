#version 450

layout(location = 0) out vec3 forward;

layout(set = 0, binding = 0) uniform ViewProjData {
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

const vec2 quadCorner[4] = {
    vec2(-1.0, -1.0),
    vec2(1.0, -1.0),
    vec2(-1.0, 1.0),
    vec2(1.0, 1.0)
};

void main() {
    vec2 pos = quadCorner[gl_VertexIndex];
    gl_Position = vec4(pos, 0.0, 1.0);

}