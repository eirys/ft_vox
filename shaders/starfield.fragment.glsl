#version 450

layout(location = 0) in     float inMoonHeight;
layout(location = 1) in     float inRandom;

layout(location = 0) out    vec4 outFragColor;

void main() {
    // Alpha depends on vertex height
    const float starIntensity = clamp((inMoonHeight + 0.05) * inRandom, 0.0, 1.0);

    outFragColor = vec4(0.8, 0.8, 0.8, starIntensity);
}