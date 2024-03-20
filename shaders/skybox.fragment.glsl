#version 450

layout(location = 0) in vec3 inForward;

layout(location = 0) out vec4 fragColor;

layout(set = 1, binding = 2) uniform samplerCube    SkyboxTex;

void main() {
    fragColor = texture(SkyboxTex, inForward);
}