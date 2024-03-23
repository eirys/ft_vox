#version 450

layout(location = 0) in vec2 inUV;
layout(location = 1) in vec3 inViewDir;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec3 inSunDir;

layout(location = 0) out vec4 outFragColor;

layout(set = 1, binding = 1) uniform sampler2DArray GameTex;

const vec3 SUN_COLOR = vec3(1.0, 1.0, 0.33);

// From Iñigo Quílez
vec3 applyFog(
    in vec3 fragColor,
    in float depth,
    in vec3 camDir,
    in vec3 sunDir
) {
    // vec3  sunColor = vec3(
    //     (gameData.sunColor & 0xFF) / 255.0,
    //     ((gameData.sunColor >> 8) & 0xFF) / 255.0,
    //     ((gameData.sunColor >> 16) & 0xFF) / 255.0);
    float fogAmount = 1.0 - exp(-depth * 0.5);
    float sunAmount = pow(max(dot(camDir, sunDir), 0.0), 8.0);
    vec3  fogColor = mix(
        vec3(0.0),
        // vec3(0.5, 0.6, 0.8), // Blueish
        // vec3(1.0, 0.8, 0.3), // Yellowish
        // sunColor,
        vec3(1.0),
        sunAmount);
    return mix(fragColor, fogColor, fogAmount);
}

float applyDiffuse(
    in vec3 normal,
    in vec3 sunDir
) {
    return max(dot(normal, sunDir), 0.1);
}

void main() {
    vec3 color = texture(GameTex, vec3(inUV, 2)).rgb;

    float diffuse = applyDiffuse(inNormal, inSunDir);

    // vec3 fog = applyFog(color, gl_FragCoord.z, inViewDir, normalize(vec3(gameData.sunPos, 1.0)));
    outFragColor = vec4(diffuse * color, 1.0);
}