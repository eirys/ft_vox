#version 450

#include "../src/engine/game/game_decl.h"
#include "../src/engine/gfx/descriptor/sets/descriptor_decl.h"
#include "../src/engine/vox_decl.h"

layout(location = 0) in vec2 inUV;

layout(location = 0) out vec4 outFragColor;

layout(set = GBUFFER_SET, binding = 0) uniform sampler2D PositionTex;
layout(set = GBUFFER_SET, binding = 1) uniform sampler2D NormalTex;
layout(set = GBUFFER_SET, binding = 2) uniform sampler2D AlbedoTex;

layout(set = PFD_SET, binding = 0) uniform GameData {
    vec2 sunPos;
    uint skyHue;
} gameData;

layout(set = PFD_SET, binding = 1) uniform Projector {
    mat4 viewProj;
} projector;

#if ENABLE_SHADOW_MAPPING
layout(set = PFD_SET, binding = 2) uniform sampler2D Shadowmap;
#endif

const float BIAS = 0.00025;
const vec3 AMBIENT_TINT = vec3(0.07, 0.07, 0.05);
const vec3 FOG_COLOR = vec3(0.4, 0.5, 0.75);

// From Iñigo Quílez
float applyFog(in float distanceToPoint) {
    const float minFogDistance = 70.0;
    const float maxFogDistance = 150.0;

    const float fog = 1.0 - (maxFogDistance - distanceToPoint) / (maxFogDistance - minFogDistance);
    return clamp(fog, 0.0, 1.0);
}

float applyShadow(in vec3 shadowCoords) {
#if ENABLE_SHADOW_MAPPING
    const float shouldDisplayShadow = step(shadowCoords.z, 1.0);
    const float shadow = step(shadowCoords.z - BIAS, texture(Shadowmap, shadowCoords.xy).r);
    return shouldDisplayShadow * shadow + (1.0 - shouldDisplayShadow) * 1.0;
#else
    return 1.0;
#endif
}

void main() {
    // Retrieve data
    const vec4 fragPos = texture(PositionTex, inUV);
    const vec4 normal = texture(NormalTex, inUV);
    const vec4 albedo = texture(AlbedoTex, inUV);

    if (albedo.a == 0.0)
        discard;

    // External data
    const vec3 sunDir = vec3(gameData.sunPos, 0.0);
    const float sunHeight = max(sunDir.y, 0.0);

    vec3 color = albedo.rgb;

    // Sky hue
    const vec3 skyHue = vec3(
        float(gameData.skyHue >> 24 & 0xFF),
        float(gameData.skyHue >> 16 & 0xFF),
        float(gameData.skyHue >> 8 & 0xFF)) / 255.0;
    color = mix(color, skyHue, 0.005);

    // Lighting
    vec3 shadowCoords = (projector.viewProj * vec4(fragPos.xyz, 1.0)).xyz;
    shadowCoords.xy = shadowCoords.xy * 0.5 + 0.5;
    const float shadow = applyShadow(shadowCoords);

    const float diffuse = max(dot(normal.xyz, sunDir), 0.0);
    const float sunAmount = min(1.0, pow(sunHeight, 2.0) + (0.5 * sunHeight));
    color = (shadow * diffuse * sunAmount + AMBIENT_TINT) * color;

    // Fog
    const vec3 fogColor = FOG_COLOR * (sunHeight * 0.9 + 0.1);
    const float fogAmount = applyFog(normal.w);
    color = mix(color, fogColor, fogAmount);

    outFragColor = vec4(color, 1.0);

}