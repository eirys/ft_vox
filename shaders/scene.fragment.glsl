#version 450

layout(location = 0) in flat uvec4 uvwc;
layout(location = 0) out vec4 fragColor;

layout(set = 0, binding = 1) uniform sampler2DArray gameTex;

void main() {
    vec3 color = vec3(
        float((uvwc.a >> 16) & 0xFF) / 255.0,
        float((uvwc.a >> 8) & 0xFF) / 255.0,
        float(uvwc.a & 0xFF) / 255.0);

    float value = texture(gameTex, vec3(uvwc.xyz)).r;

    fragColor = vec4(value * color, 1.0);
}