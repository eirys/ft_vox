#version 450
#define VOX_SKY_LAYOUT

#include "../src/engine/gfx/descriptor/sets/descriptor_decl.h"
#include "../src/engine/game/game_decl.h"

layout(location = 0) out float outIntensity;
// layout(location = 1) out float outRandom;
layout(location = 1) out float outHeight;

layout(push_constant) uniform Camera {
    mat4 view;
    mat4 proj;
} camera;

layout(set = PFD_SET, binding = 0) uniform GameData {
    vec2 sunPos;
} gameData;

layout(set = WORLD_SET, binding = 1) uniform sampler2DArray NoiseTex;

const vec3 X_AXIS = vec3(1.0, 0.0, 0.0);
const vec3 Y_AXIS = vec3(0.0, 1.0, 0.0);
const vec3 Z_AXIS = vec3(0.0, 0.0, 1.0);
const float PI = 3.141592;
const vec3 VERTEX_POS[4] = {
    vec3(-1.0, -1.0, 0.0), // r
    vec3( 1.0, -1.0, 0.0), // g
    vec3(-1.0,  1.0, 0.0), // b
    vec3( 1.0,  1.0, 0.0), // w
};

// Generate a rotation matrix from an angle (radians) and an axis
mat4 rotate(in float angle, in vec3 axis) {
    const float c = cos(angle);
    const float s = sin(angle);
    const float oc = 1.0 - c;
    const vec3 naxis = normalize(axis);

    return mat4(
        oc * naxis.x * naxis.x + c,
        oc * naxis.x * naxis.y - naxis.z * s,
        oc * naxis.z * naxis.x + naxis.y * s,
        0.0,

        oc * naxis.x * naxis.y + naxis.z * s,
        oc * naxis.y * naxis.y + c,
        oc * naxis.y * naxis.z - naxis.x * s,
        0.0,

        oc * naxis.z * naxis.x - naxis.y * s,
        oc * naxis.y * naxis.z + naxis.x * s,
        oc * naxis.z * naxis.z + c,
        0.0,

        0.0, 0.0, 0.0, 1.0
    );
}

// Generate rotation matrix from cos and sin input
mat4 rotate2(in vec2 cs, in vec3 axis) {
    const float oc = 1.0 - cs.x;
    const vec3 naxis = normalize(axis);

    return mat4(
        oc * naxis.x * naxis.x + cs.x,
        oc * naxis.x * naxis.y - naxis.z * cs.y,
        oc * naxis.z * naxis.x + naxis.y * cs.y,
        0.0,

        oc * naxis.x * naxis.y + naxis.z * cs.y,
        oc * naxis.y * naxis.y + cs.x,
        oc * naxis.y * naxis.z - naxis.x * cs.y,
        0.0,

        oc * naxis.z * naxis.x - naxis.y * cs.y,
        oc * naxis.y * naxis.z + naxis.x * cs.y,
        oc * naxis.z * naxis.z + cs.x,
        0.0,

        0.0, 0.0, 0.0, 1.0
    );
}

// Generate a translation matrix from a direction
mat4 translate(in vec3 dir) {
    mat4 mat = mat4(1.0);
    mat[3].xyz = dir;
    return mat;
}

mat4 scale(in float val) {
    return mat4(
        val, 0.0, 0.0, 0.0,
        0.0, val, 0.0, 0.0,
        0.0, 0.0, val, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

vec3 getWorldAxis(in mat4 inverseModel, in vec3 localAxis) {
    return (inverseModel * vec4(localAxis, 0.0)).xyz;
}

mat4 getModel(in vec2 moonDir, in vec3 random) {
    const vec2 displacementAmplitude = vec2(3.0, 0.9);

    // Align the star on the moon
    const mat4 alignOnMoon = rotate(0.5 * PI, Y_AXIS);

    // Rotate around the moon
    const mat4 moonCentered = rotate(random.z * PI * 2.0 * displacementAmplitude.x, getWorldAxis(transpose(alignOnMoon), X_AXIS));

    // Draw the star away from the moon
    const mat4 drawAway = rotate((random.y * 0.93 + 0.07) * displacementAmplitude.y * PI, X_AXIS);

    // Angle of the star
    const mat4 starAngle = rotate(random.x * PI, Z_AXIS);

    // Follow the moon
    const mat4 starHeightInSky = rotate2(moonDir, getWorldAxis(transpose(starAngle) * transpose(drawAway) * transpose(moonCentered) * transpose(alignOnMoon), Z_AXIS));

    // Move star to the sky
    const float skyDist = 150.0;
    const mat4 starDistance = translate(vec3(0.0, 0.0, skyDist));

    // Randomize the size of the star
    const mat4 starSize = scale((random.x * 0.5 + 0.5) * 0.8);

    return alignOnMoon
         * moonCentered
         * drawAway
         * starAngle
         * starHeightInSky
         * starDistance
         * starSize;
}

void main() {
    const vec2 moonDir = vec2(gameData.sunPos.x, -gameData.sunPos.y);
    const uvec2 noisemapSize = uvec2(textureSize(NoiseTex, 0).xy);
    const vec2 noiseCoord = vec2(gl_InstanceIndex % noisemapSize.x, gl_InstanceIndex / noisemapSize.x) / vec2(noisemapSize);
    const vec3 random = vec3(
        texture(NoiseTex, vec3(noiseCoord, 0)).r,
        texture(NoiseTex, vec3(noiseCoord, 1)).r,
        texture(NoiseTex, vec3(noiseCoord, 2)).r);

    const mat4 model = getModel(moonDir, random);
    const vec4 vertex = vec4(VERTEX_POS[gl_VertexIndex], 1.0);
    const vec4 worldPos = model * vertex;

    const float randomFactor = texture(NoiseTex, vec3(noiseCoord, 3)).r * 0.8 + 0.2;
    float moonHeight = max(moonDir.y, 0.0);

    outIntensity = randomFactor * moonHeight * moonHeight;
    outHeight = (camera.proj * worldPos).y;

    gl_Position = camera.proj * mat4(mat3(camera.view)) * worldPos;
}