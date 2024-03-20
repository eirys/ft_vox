#version 450

layout(location = 0) in vec3 inUV;

layout(location = 0) out vec4 fragColor;

layout(set = 1, binding = 1) uniform sampler2DArray GameTex;

void main() {
    fragColor = texture(GameTex, inUV);
}