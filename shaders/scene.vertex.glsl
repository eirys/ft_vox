#version 450

layout(location = 0) out vec3 outColor;

layout(set = 0, binding = 0) uniform UniformBufferObject {
    uint color;
} ubo;

const vec2 quadpos[4] = {
    { -1.0, -1.0 },
    {  1.0, -1.0 },
    { -1.0,  1.0 },
    {  1.0,  1.0 },
};

void main() {
    gl_Position = vec4(quadpos[gl_VertexIndex], 0.0, 1.0);
    outColor = vec3(
        float((ubo.color >> 16) & 0xFF) / 255.0,
        float((ubo.color >> 8) & 0xFF) / 255.0,
        float(ubo.color & 0xFF) / 255.0
    );
}