#version 450

#include "../src/engine/game/game_decl.h"
#include "../src/engine/gfx/descriptor/sets/descriptor_decl.h"
#include "../src/engine/vox_decl.h"

layout(location = 0) in vec2 inUV;

layout(location = 0) out vec4 outColor;

layout(set = PFD_SET, binding = 0) uniform GameData {
    vec2 sunPos;
    uint skyHue;
    uint debugIndex;
} gameData;

layout(set = GBUFFER_SET, binding = 0) uniform sampler2D PositionTex;
layout(set = GBUFFER_SET, binding = 1) uniform sampler2D NormalTex;
layout(set = GBUFFER_SET, binding = 2) uniform sampler2D AlbedoTex;

// #if ENABLE_SHADOW_MAPPING
//     layout(set = PFD_SET, binding = 2) uniform sampler2D Shadowmap;
// #endif

void main() {
    outColor = vec4(1.0);

    switch (gameData.debugIndex) {
        case 0:
            vec4 fragPos = texture(PositionTex, inUV);
            outColor.rgb = fragPos.rgb / fragPos.w;
            break;
        case 1:
            outColor.rgb = texture(NormalTex, inUV).rgb;
            break;
        case 2:
            outColor.rgb = texture(AlbedoTex, inUV).rgb;
            break;
        default:
            break;
    }

// #if ENABLE_SHADOW_MAPPING
//     float depthValue = texture(Shadowmap, inUV).r;
//     outColor = vec4(vec3(depthValue), 1.0);
// #endif
}