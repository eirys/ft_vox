#version 450

layout(location = 0) in vec3 inUVW;
layout(location = 1) in vec3 inSunDir;

layout(location = 0) out vec4 outFragColor;

layout(set = 1, binding = 2) uniform samplerCube    SkyboxTex;

// Apply sun lighting depending on the angle between the eye direction and the sun direction
float applySun(in vec3 eyeDir, in vec3 sunDir) {
    float sunDot = clamp(dot(eyeDir, sunDir), 0.0, 1.0);
    float sunAmount =
        0.02 * pow(sunDot, 8.0) + // Outer glow
        0.5 * pow(sunDot, 64.0) + // Inner glow
        0.8 * pow(sunDot, 256.0); // Sun disk
    return sunAmount;
}

const float TINT_INTENSITY = 0.98;
const vec3 SUN_COLOR = vec3(1.0, 1.0, 0.33);
const vec3 DIMMED_SUN_COLOR = vec3(1.0, 0.3, 0.0);

const vec3 DAY_TINT = vec3(0.6, 0.8, 1.0);
const vec3 NIGHT_TINT = vec3(0.0, 0.0, 0.01);
const vec3 SUNSET_TINT = vec3(0.9, 0.3, 0.0);
const vec3 MOON_TINT = vec3(0.2, 0.2, 0.3);
const vec3 VOID_COLOR = vec3(0.0);

vec3 computeSkyColor(in vec3 defaultColor, in float sunHeight, in float horizonLine) {
    vec3 skyColor;

    // Tint skybox depending on day time (blend mode: multiply)
    const vec3 skyboxColor = mix(NIGHT_TINT, DAY_TINT, sunHeight) * defaultColor;

    // Add horizon hue
    const vec3 horizonColor = mix(NIGHT_TINT, DAY_TINT, sunHeight);
    skyColor = mix(skyboxColor, horizonColor, horizonLine);

    // Add sun glow
    const vec3 glowColor = mix(MOON_TINT, SUNSET_TINT, sunHeight);
    skyColor = mix(skyColor, glowColor, pow(horizonLine, 8.0) * (1.0 - sunHeight));

    // Add void
    skyColor = mix(skyColor, VOID_COLOR, pow(horizonLine, 16.0));

    // Apply tint
    skyColor = mix(skyboxColor, skyColor, TINT_INTENSITY);

    return skyColor;
}

void main() {
    const vec3 eyeDir = normalize(inUVW);
    const float sunHeight = max(inSunDir.y, 0.0);

    const float horizonLine = 1.0 - clamp(eyeDir.y + 0.15, 0.0, 1.0); // Offset eye height to lower horizon

    const vec3 skyTex = texture(SkyboxTex, inUVW).rgb;
    vec3 skyColor = computeSkyColor(skyTex, sunHeight, horizonLine);

    const vec3 sunColor = mix(SUN_COLOR, DIMMED_SUN_COLOR, horizonLine);
    const float sunAmount = (1.0 - horizonLine) * applySun(eyeDir, inSunDir);

    outFragColor = vec4(mix(skyColor, sunColor, sunAmount), 1.0);
}