#version 450

layout(set = 0, binding = 0) uniform UniformBufferObject {
    uint color;
} ubo;

layout(location = 0) out vec3 outColor;

const vec2[4] quadpos = {
    vec2(-1.0, -1.0),
    vec2(1.0, -1.0),
    vec2(-1.0, 1.0),
    vec2(1.0, 1.0)
};

void main() {
    gl_Position = vec4(0.0, 0.0, 0.5, 1.0);
    outColor = vec3(
        ((ubo.color >> 16) & 0xFF) / 255.0,
        ((ubo.color >> 8) & 0xFF) / 255.0,
        (ubo.color & 0xFF) / 255.0
    );
}