#version 450
#define VOX_SSAO_BLUR_LAYOUT

#include "../src/engine/gfx/descriptor/sets/descriptor_decl.h"
#include "../src/engine/game/game_decl.h"

layout(location = 0) in vec2 inUV;

layout(location = 0) out float outFragColor;

layout(set = SSAO_BLUR_SET, binding = 0) uniform sampler2D SsaoTex;

// --------------------

const int blurRange = 2;

// --------------------

void main() {
    int count = 0;
    vec2 texelSize = 1.0 / vec2(textureSize(SsaoTex, 0));
    float result = 0.0;
    for (int x = -2; x < 2; ++x) {
        for (int y = -2; y < 2; ++y) {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(SsaoTex, inUV + offset).r;
        }
    }
    outFragColor = result / 16.0;
}