#version 450

layout(location = 0) in vec3 inUVW;
layout(location = 1) in vec3 inSunDir;
// layout(location = 2) in vec3 inSunColor;

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
const vec3 DIMMED_SUN_COLOR = vec3(0.8, 0.3, 0.0);

const vec3 DAY_TINT = vec3(0.6, 0.8, 1.0);
const vec3 NIGHT_TINT = vec3(0.0005, 0.0, 0.075);
const vec3 SUNSET_TINT = vec3(0.9, 0.6, 0.0);
const vec3 VOID_TINT = vec3(0.0);

vec3 computeSkyColor(in float sunHeight, in float horizonLine) {
    vec3 skyColor;

    const vec3 skyTex = texture(SkyboxTex, inUVW).rgb;

    // Tint skybox depending on day time
    const vec3 skyboxColor = mix(NIGHT_TINT, DAY_TINT, sunHeight) * skyTex;
    const vec3 horizonColor = mix(NIGHT_TINT, SUNSET_TINT, sunHeight);

    // Add horizon hue
    skyColor = mix(skyboxColor, horizonColor, (1.0 - sunHeight) * horizonLine);

    // Add void
    skyColor = mix(skyColor, VOID_TINT, pow(horizonLine, 5.0));

    // Apply tint
    skyColor = mix(skyboxColor, skyColor, TINT_INTENSITY);

    return skyColor;
}

void main() {
    const vec3 eyeDir = normalize(inUVW);
    const float sunHeight = max(dot(inSunDir, vec3(0.0, 1.0, 0.0)), 0.0);

    float horizonLine = 1.0 - clamp(eyeDir.y + 0.07, 0.0, 1.0); // Offset eye height to avoid black sky at the horizon

    vec3 skyColor = computeSkyColor(sunHeight, horizonLine);

    const vec3 sunColor = mix(SUN_COLOR, DIMMED_SUN_COLOR, horizonLine);
    const float sunAmount = (1.0 - horizonLine) * applySun(eyeDir, inSunDir);

    outFragColor = vec4(mix(skyColor, sunColor, sunAmount), 1.0);
}