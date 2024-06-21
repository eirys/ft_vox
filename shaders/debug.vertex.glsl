#version 450
#define VOX_SCENE_LAYOUT

layout(location = 0) out vec2 outUV;
layout(location = 1) out mat3 outInvView;

layout(push_constant) uniform Camera {
    mat4 view;
    mat4 proj;
} camera;

const float scale = 0.15;
const float ratio = 1.5;
const float translation = 0.7;

const vec2 VERTICES[4] = {
    { -1.0, -1.0 },
    { -1.0,  1.0 },
    {  1.0, -1.0 },
    {  1.0,  1.0 },
};

void main() {
    const vec2 quadVertex = VERTICES[gl_VertexIndex] * vec2(1.0, ratio) * scale + vec2(translation);

    outUV = VERTICES[gl_VertexIndex] * 0.5 + 0.5;
    outInvView = transpose(inverse(mat3(camera.view)));

    gl_Position = vec4(quadVertex, 0.0, 1.0);
}