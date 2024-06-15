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

#if ENABLE_SHADOW_MAPPING
    layout(set = PFD_SET, binding = 2) uniform sampler2D Shadowmap;
#endif

void main() {
    outColor = vec4(1.0);

    switch (gameData.debugIndex) {
        case 1:
            outColor.rgb = texture(PositionTex, inUV).www;
            break;
        case 2:
            outColor.rgb = texture(NormalTex, inUV).rgb;
            break;
        case 3:
            outColor.rgb = texture(AlbedoTex, inUV).rgb;
            break;
        case 4:
#if ENABLE_SHADOW_MAPPING
            outColor.rgb = texture(Shadowmap, inUV).rrr;
#else
            outColor.rgb = vec3(0.0);
#endif
            break;
        default:
            break;
    }
}