#version 450
#define VOX_SCENE_LAYOUT

#include "../src/engine/game/game_decl.h"
#include "../src/engine/gfx/descriptor/sets/descriptor_decl.h"
#include "../src/engine/vox_decl.h"

layout(location = 0) in vec2 inUV;

layout(location = 0) out vec4 outColor;

// ------------------------------------

layout(set = PFD_SET, binding = 0) uniform GameData {
    vec2 dummy;
    uint dummy2;
    uint debugIndex;
} gameData;

layout(set = GBUFFER_SET, binding = 0) uniform sampler2D PositionTex;
layout(set = GBUFFER_SET, binding = 1) uniform sampler2D NormalTex;
layout(set = GBUFFER_SET, binding = 2) uniform sampler2D AlbedoTex;

#if ENABLE_SSAO
layout(set = GBUFFER_SET, binding = 3) uniform sampler2D SsaoTex;
layout(set = GBUFFER_SET, binding = 4) uniform sampler2D BlurTex;
#endif

#if ENABLE_SHADOW_MAPPING
    layout(set = PFD_SET, binding = 2) uniform sampler2D Shadowmap;
#endif

// ------------------------------------

void main() {
    outColor = vec4(1.0);

    switch (gameData.debugIndex) {
        case 1: // Position
            outColor.rgb = texture(PositionTex, inUV).rgb * 0.01;
            break;
        case 2: // Depth (scaled to 0-1 range)
            outColor.rgb = texture(PositionTex, inUV).www / (CHUNK_SIZE * RENDER_DISTANCE * 0.5);
            break;
        case 3: // Normal
            outColor.rgb = texture(NormalTex, inUV).rgb;
            break;
        case 4: // Albedo
            outColor.rgb = texture(AlbedoTex, inUV).rgb;
            break;

#if ENABLE_SHADOW_MAPPING
        case 5: // Shadowmap
            outColor.rgb = texture(Shadowmap, inUV).rrr;
            break;
#endif

#if ENABLE_SSAO
        case 6: // Ssao
            outColor.rgb = texture(SsaoTex, inUV).rrr;
            break;

        case 7: // Ssao (blur)
            outColor.rgb = texture(BlurTex, inUV).rrr;
            break;
#endif

        default:
            outColor.rgb = vec3(0.0);
            break;
    }
}