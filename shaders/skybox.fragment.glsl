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

void main() {
    const vec3 eyeDir = normalize(inUVW);
    const float sunHeight = max(dot(inSunDir, vec3(0.0, 1.0, 0.0)), 0.0);

    // too lazy to do proper textures atm
    const vec3 skyTex = texture(SkyboxTex, inUVW).rgb;
    const vec3 skyboxColor = vec3(0.6, 0.8, 1.0) * skyTex;

    const vec3 sunsetTint = vec3(0.6, 0.3, 0.0);
    const vec3 voidTint = vec3(0.0);

    // Add sunset hue
    float t = 1.0 - max(eyeDir.y, 0.0);
    vec3 tint = mix(skyboxColor, sunsetTint, mix(t, 0.0, sunHeight));

    // Add void
    t = pow(t, 5.0);
    tint = mix(tint, voidTint, t);
    vec3 skyColor = mix(skyboxColor, tint, TINT_INTENSITY);

    const vec3 sunColor = mix(SUN_COLOR, DIMMED_SUN_COLOR, t);
    const float sunAmount = (1.0 - t) * applySun(eyeDir, inSunDir);

    outFragColor = vec4(mix(skyColor, SUN_COLOR, sunAmount), 1.0);
}