#version 450

layout(location = 0) out vec4 uvwc;

layout(set = 0, binding = 0) uniform UniformBufferObject {
    uint data;
} ubo;

const vec2 quadpos[4] = {
    { -1.0, -1.0 },
    {  1.0, -1.0 },
    { -1.0,  1.0 },
    {  1.0,  1.0 },
};

const vec3 colors[4] = {
    {0.,0.,0.},
    {1.,0.,0.},
    {0.,1.,0.},
    {0.,0.,1.},
};

const vec2 uvs[4] = {
    { 0.0, 0.0 },
    { 0.0, 1.0 },
    { 1.0, 1.0 },
    { 1.0, 0.0 },
};

void main() {
    gl_Position = vec4(quadpos[gl_VertexIndex], 0.0, 1.0);

    uint index = ubo.data >> 24;
    uint color = ubo.data & 0xFFffFF;

    uvwc = vec4(
        quadpos[gl_VertexIndex] * 0.5 + vec2(0.5),
        float(index),
        // colors[gl_VertexIndex],
        float(color));
}