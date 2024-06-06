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

#if 0
float applyShadowPCF(in vec3 shadowCoords) {
    const vec2 texelSize = 1.0 / textureSize(Shadowmap, 0);

    float shadow = 0.0;

    for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
            const vec2 offset = vec2(x, y) * texelSize;
            const float depthValue = texture(Shadowmap, shadowCoords.xy + offset).r;
            const float distanceToLight = shadowCoords.z;
            shadow += step(distanceToLight - 0.005, depthValue);
        }
    }

    return shadow / 9.0;
}
#endif

float applyShadow(in vec3 shadowCoords) {
#if ENABLE_SHADOW_MAPPING
    const float depthValue = texture(Shadowmap, shadowCoords.xy).r;
    const float distanceToLight = shadowCoords.z;

    const float shouldDisplayShadow = step(distanceToLight, 1.0);
    return shouldDisplayShadow * step(distanceToLight - 0.005, depthValue) + (1.0 - shouldDisplayShadow) * 1.0;
#endif
    return 1.0;
}

float applyDiffuse(in vec3 normal, in vec3 sunDir) {
    const float illumination = max(dot(normal, sunDir), 0.0);
    return illumination;
}

void main() {
    const vec3 sunDir = vec3(gameData.sunPos, 0.0);
    const float sunHeight = max(sunDir.y, 0.0);
    const float sunHeightFactor = min(1.0, pow(sunHeight, 2.0) + (0.5 * sunHeight));

    vec3 color = texture(GameTex, inUVW).rgb;

    // Sky hue
    const vec3 skyHue = vec3(
        float(gameData.skyHue >> 24 & 0xFF),
        float(gameData.skyHue >> 16 & 0xFF),
        float(gameData.skyHue >> 8 & 0xFF)) / 255.0;
    color = mix(color, skyHue, 0.005);

    // Lighting
    const float shadow = applyShadow(inShadowCoords);
    const float diffuseLight = sunHeightFactor * applyDiffuse(inNormal, sunDir);
    color = (shadow * diffuseLight + AMBIENT_TINT) * color;

    // Fog
    const vec3 fogColor = FOG_COLOR * (sunHeight * 0.9 + 0.1);
    const float fogAmount = applyFog(gl_FragCoord.z / gl_FragCoord.w);
    color = mix(color, fogColor, fogAmount);

    outFragColor = vec4(color, 1.0);
}