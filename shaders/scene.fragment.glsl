#version 450

layout(location = 0) in vec4 uvwc;
layout(location = 0) out vec4 fragColor;

layout(set = 0, binding = 1) uniform sampler2DArray gameTex;

void main() {
    vec3 color = vec3(
        ((uint(uvwc.a) >> 16)   & 0xFF) / 255.0,
        ((uint(uvwc.a) >> 8)    & 0xFF) / 255.0,
        ( uint(uvwc.a)          & 0xFF) / 255.0);

    float noise = texture(gameTex, uvwc.xyz).r;

    fragColor = vec4(color * noise.xxx, 1.0);
}