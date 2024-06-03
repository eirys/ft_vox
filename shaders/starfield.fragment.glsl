#version 450

layout(location = 0) in     float inIntensity;
layout(location = 1) in     float inRandom;
layout(location = 2) in     float inHeight;

layout(location = 0) out    vec4 outFragColor;

void main() {
    // Useless in the future, when full world is implemented
    if (inHeight > 14.0) {
        discard;
    }
    // Alpha depends on vertex height and random value
    const float starIntensity = clamp(inIntensity * inRandom, 0.0, 1.0);

    outFragColor = vec4(0.9, 0.9, 0.9, starIntensity);
}