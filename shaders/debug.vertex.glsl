#version 450

layout(location = 0) out vec2 outUV;

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

    gl_Position = vec4(quadVertex, 0.0, 1.0);
    outUV = VERTICES[gl_VertexIndex] * 0.5 + 0.5;
}