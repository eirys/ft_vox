#version 450

#include "../src/engine/game/game_decl.h"
#include "../src/engine/gfx/descriptor/sets/descriptor_decl.h"

layout(location = 0) in vec2 inUV;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inSunDir;

layout(location = 0) out vec4 outFragColor;

layout(set = WORLD_SET, binding = 1) uniform sampler2DArray GameTex;

const vec3 SUN_COLOR = vec3(1.0, 1.0, 0.33);
const vec3 MOON_COLOR = vec3(0.5, 0.5, 0.5);
const vec3 FOG_COLOR = vec3(0.3, 0.4, 0.6);

// From Iñigo Quílez
float applyFog(in float distanceToPoint) {
    const float minFogDistance = 0.0;
    const float maxFogDistance = 200.0;

    return 1.0 - (maxFogDistance - distanceToPoint) / (maxFogDistance - minFogDistance);
}

float applyDiffuse(in vec3 normal, in vec3 sunDir) {
    return max(dot(normal, sunDir), 0.05);
}

void main() {
    const float sunHeight = max(inSunDir.y, 0.0);

    vec3 color = texture(GameTex, vec3(inUV, 2)).rgb;

    const float ambientFactor = 0.1;
    const vec3 ambientColor = mix(MOON_COLOR, SUN_COLOR, sunHeight);

    // Fog
    const float fogAmount = applyFog(gl_FragCoord.z / gl_FragCoord.w);
    const vec3 fogColor = mix(FOG_COLOR, ambientColor, ambientFactor);
    color = mix(color, fogColor, fogAmount);

    // Lighting
    const float diffuseFactor = applyDiffuse(inNormal, inSunDir);
    color = diffuseFactor * mix(color, ambientColor, 0.01);

    outFragColor = vec4(color, 1.0);
}