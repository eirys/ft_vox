#version 450
#define VOX_SCENE_LAYOUT

#include "../src/engine/game/game_decl.h"
#include "../src/engine/gfx/descriptor/sets/descriptor_decl.h"
#include "../src/engine/vox_decl.h"

layout(location = 0) in vec2 inUV;

layout(location = 0) out vec4 outFragColor;

// ------------------------------------

layout(set = GBUFFER_SET, binding = 0) uniform sampler2D PositionTex;
layout(set = GBUFFER_SET, binding = 1) uniform sampler2D NormalTex;
layout(set = GBUFFER_SET, binding = 2) uniform sampler2D AlbedoTex;

#if ENABLE_SSAO
layout(set = GBUFFER_SET, binding = 4) uniform sampler2D BlurSsaoTex;
#endif

layout(push_constant) uniform Camera {
    mat4 view;
    mat4 proj;
} camera;

layout(set = PFD_SET, binding = 0) uniform GameData {
    vec2 sunPos;
    uint skyHue;
} gameData;

#if ENABLE_SHADOW_MAPPING
layout(set = PFD_SET, binding = 1) uniform Projector {
    mat4 viewProj;
} projector;

layout(set = PFD_SET, binding = 2) uniform sampler2D Shadowmap;
#endif

// ------------------------------------

const float BIAS = 0.00025;
const vec3 FOG_COLOR = vec3(0.4, 0.5, 0.75);

// ------------------------------------

// From Iñigo Quílez
float applyFog(in float distanceToPoint) {
    const float minFogDistance = 100.0;
    const float maxFogDistance = 200.0;

    const float fog = 1.0 - (maxFogDistance - distanceToPoint) / (maxFogDistance - minFogDistance);
    return clamp(fog, 0.0, 1.0);
}

#if ENABLE_SHADOW_MAPPING
float applyShadow(in vec4 position) {
    vec3 shadowCoords = (projector.viewProj * position).xyz;
    shadowCoords.xy = shadowCoords.xy * 0.5 + 0.5;
    const float shouldDisplayShadow = step(shadowCoords.z, 1.0);
    const float shadow = step(shadowCoords.z - BIAS, texture(Shadowmap, shadowCoords.xy).r);
    return shouldDisplayShadow * shadow + (1.0 - shouldDisplayShadow) * 1.0;
}
#endif

void main() {
    // Gbuffer
    const vec4 position = texture(PositionTex, inUV);
    const vec3 normal = texture(NormalTex, inUV).rgb;
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
    const float intensity = min(1.0, pow(sunHeight, 2.0) + (0.5 * sunHeight));
    const float diffuse = max(dot(normal, sunDir), 0.0);
    const float ambient = mix(0.05, 0.3, sunHeight);
    float lighting = diffuse * intensity;
#if ENABLE_SHADOW_MAPPING
    const float shadow = applyShadow(vec4(position.xyz, 1.0));
    lighting *= shadow;
#endif
    lighting = clamp(lighting + ambient, 0.0, 1.0);
#if ENABLE_SSAO
    const float ssao = texture(BlurSsaoTex, inUV).r;
    lighting *= ssao;
#endif
    color *= lighting;

    // Fog
    const vec3 fogColor = FOG_COLOR * (sunHeight * 0.9 + 0.1);
    const float fogAmount = applyFog(position.w);
    color = mix(color, fogColor, fogAmount);

    outFragColor = vec4(color, 1.0);
}