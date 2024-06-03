#version 450

#include "../src/engine/game/game_decl.h"
#include "../src/engine/gfx/descriptor/sets/descriptor_decl.h"
#include "../src/engine/vox_decl.h"

layout(location = 0) in vec3 inUVW;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inShadowCoords;

layout(location = 0) out vec4 outFragColor;

layout(set = PFD_SET, binding = 1) uniform GameData {
    vec2 sunPos;
    uint skyHue;
} gameData;

#if ENABLE_SHADOW_MAPPING
    layout(set = PFD_SET, binding = 3) uniform sampler2D Shadowmap;
#endif

// layout(set = WORLD_SET, binding = 1) uniform sampler2DArray GameTex;
layout(set = WORLD_SET, binding = 0) uniform sampler2DArray GameTex;

const vec3 AMBIENT_TINT = vec3(0.07, 0.07, 0.05);
const vec3 FOG_COLOR = vec3(0.3, 0.4, 0.6);

// From Iñigo Quílez
float applyFog(in float distanceToPoint) {
    const float minFogDistance = 50.0;
    const float maxFogDistance = 200.0;

    const float fog = 1.0 - (maxFogDistance - distanceToPoint) / (maxFogDistance - minFogDistance);
    return clamp(fog, 0.0, 1.0);
}

float isFragInShadow(vec3 shadowCoords) {
#if ENABLE_SHADOW_MAPPING
    if (shadowCoords.z > 0.0 && shadowCoords.z < 1.0) {
        float dist = texture(Shadowmap, shadowCoords.xy).r;
        if (dist + 0.001 < shadowCoords.z)
            return 1.0;
    }
#endif
    return 0.0;
}

float applyDiffuse(in vec3 normal, in vec3 sunDir, in float sunHeight) {
    if (sunHeight <= 0.0)
        return 0.0;

    const float sunHeightFactor = min(1.0, pow(sunHeight, 2.0) + (0.5 * sunHeight));
    const float illumination = max(dot(normal, sunDir), 0.0);

    const float isInShadow = isFragInShadow(inShadowCoords);// * step(0.0, sunHeight);

    return
        mix(
              mix(0.05, illumination, sunHeightFactor),
             0.0,
              isInShadow
        );
}

void main() {
    const vec3 sunDir = vec3(gameData.sunPos, 0.0);
    const float sunHeight = max(sunDir.y, 0.0);

    vec3 color = texture(GameTex, inUVW).rgb;

    // Sky hue
    const vec3 skyHue = vec3(
        float(gameData.skyHue >> 24 & 0xFF),
        float(gameData.skyHue >> 16 & 0xFF),
        float(gameData.skyHue >> 8 & 0xFF)) / 255.0;
    color = mix(color, skyHue, 0.005);

    // Lighting
    const float diffuseFactor = applyDiffuse(inNormal, sunDir, sunHeight);
    color = (diffuseFactor + AMBIENT_TINT) * color;

    // Fog
    const vec3 fogColor = FOG_COLOR * (sunHeight * 0.9 + 0.1);
    const float fogAmount = applyFog(gl_FragCoord.z / gl_FragCoord.w);
    color = mix(color, fogColor, fogAmount);

    outFragColor = vec4(color, 1.0);
}