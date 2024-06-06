#version 450

#include "../src/engine/game/game_decl.h"
#include "../src/engine/vox_decl.h"
#include "../src/engine/gfx/descriptor/sets/descriptor_decl.h"

layout(location = 0) in vec3 inUVW;

layout(location = 0) out vec4 outFragColor;

layout(set = PFD_SET, binding = 1) uniform GameData {
    vec2 sunPos;
    uint skyHue;
} gameData;

#if ENABLE_CUBEMAP
// layout(set = WORLD_SET, binding = 3) uniform samplerCube    SkyboxTex;
layout(set = WORLD_SET, binding = 2) uniform samplerCube SkyboxTex;
#endif

const float TINT_INTENSITY = 0.02;

const vec3 DEFAULT_SKY_COLOR = vec3(0.5, 0.5, 1.0);
const vec3 SUN_COLOR = vec3(1.0, 1.0, 0.33);
const vec3 MOON_COLOR = vec3(0.8, 0.8, 0.9);
const vec3 VOID_COLOR = vec3(0.0);

const vec3 DAY_TINT = vec3(0.6, 0.8, 1.0);
const vec3 SUNSET_TINT = vec3(0.9, 0.3, 0.0);

const vec3 NIGHT_TINT = vec3(0.0, 0.0, 0.01);
const vec3 MOONLIGHT_TINT = vec3(0.2, 0.2, 0.3);

vec3 applySun(
    in vec3 skyColor,
    in vec3 skyboxPoint,
    in vec3 sunDir,
    in float horizonLine
) {
    const vec3 sunColor = mix(SUN_COLOR, SUNSET_TINT, horizonLine);
    const float sunIntensity = clamp(dot(skyboxPoint, sunDir), 0.0, 1.0);
    const float sun =
        (0.1 * pow(sunIntensity, 8.0)) +   // Outer glow
        (0.5 * pow(sunIntensity, 32.0)) +   // More glow
        (1.0 * pow(sunIntensity, 256.0)) +  // MORE glow
        step(0.992, sunIntensity);          // Sun disk

    const float horizonEffect = pow(horizonLine, 16.0);
    const float sunAmount = clamp((1.0 - horizonEffect) * sun, 0.0, 1.0);

    return mix(skyColor, sunColor, sunAmount);
}

vec3 applyMoon(
    in vec3 skyColor,
    in vec3 skyboxPoint,
    in vec3 sunDir,
    in float horizonLine
) {
    const float moonIntensity = clamp(dot(skyboxPoint, -sunDir), 0.0, 1.0);
    const float moon =
        (0.02 * pow(moonIntensity, 8.0)) +  // Outer glow
        (0.2 * pow(moonIntensity, 128.0)) + // More glow
        (0.5 * step(0.99, moonIntensity));  // Moon disk
    const float moonlightAmount = clamp((1.0 - horizonLine) * moon, 0.0, 1.0);

    return mix(skyColor, MOON_COLOR, moonlightAmount);
}

vec3 computeSkyColor(in vec3 defaultColor, in float sunHeight, in float horizonLine) {
    const vec3 skyTint = mix(NIGHT_TINT, DAY_TINT, sunHeight);
    const vec3 tintedSky = defaultColor * skyTint;

    vec3 skyColor;

    // Add horizon hue (amplify sky tint at the horizon line)
    skyColor = mix(tintedSky, skyTint, horizonLine);

    // Add skylight glow
    const vec3 glowColor = mix(MOONLIGHT_TINT, SUNSET_TINT, sunHeight);
    skyColor = mix(skyColor, glowColor, pow(horizonLine, 8.0) * (1.0 - sunHeight));

    // Add void
    skyColor = mix(skyColor, VOID_COLOR, pow(horizonLine, 16.0));

    return mix(skyColor, tintedSky, TINT_INTENSITY);
}

void main() {
    const vec3 skyboxPoint = normalize(inUVW);
    const vec3 sunDir = vec3(gameData.sunPos, 0.0);
    const float sunHeight = max(sunDir.y, 0.0);
    const float horizonLine = 1.0 - clamp(skyboxPoint.y + 0.15, 0.0, 1.0); // Offset height to lower horizon

    vec3 skyColor;

    // Sample skybox texture
#if ENABLE_CUBEMAP
    skyColor = texture(SkyboxTex, inUVW).rgb;
#else
    skyColor = DEFAULT_SKY_COLOR;
#endif

    // Add atmosphere
    skyColor = computeSkyColor(skyColor, sunHeight, horizonLine);

    // Add moon
    skyColor = applyMoon(skyColor, skyboxPoint, sunDir, horizonLine);

    // Add sun
    skyColor = applySun(skyColor, skyboxPoint, sunDir, horizonLine);

    outFragColor = vec4(skyColor, 1.0);
}