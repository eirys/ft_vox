#version 450

#include "../src/engine/gfx/descriptor/sets/descriptor_decl.h"
#include "../src/engine/game/game_decl.h"

layout(location = 0) out float outIntensity;

// 1 quad = 1 noise value:
// generate rotation matrix
// generate translation matrix
// 1 quad = 1 color

layout(set = MVP_SET, binding = 0) uniform ViewProj {
    mat4 view;
    mat4 proj;
} viewProj;

layout(set = MVP_SET, binding = 1) uniform GameData {
    vec2 sunPos;
} gameData;

layout(set = WORLD_SET, binding = 2) uniform sampler2D NoiseTex;

// Generate a rotation matrix from an angle (radians) and an axis
mat4 rotate(in float angle, in vec3 axis) {
    mat4 mat = mat4(1.0);

    const float c = cos(angle);
    const float s = sin(angle);
    const float oc = 1.0 - c;
    const vec3 naxis = normalize(axis);

    mat *= mat4(
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
    return mat;
}

// Generate a translation matrix from a direction
mat4 translate(in vec3 dir) {
    mat4 mat = mat4(1.0);
    mat[3].xyz = dir;
    return mat;
}

// Generate a scale matrix from a scale vector
mat4 scale(in vec3 scale) {
    mat4 mat = mat4(0.0);
    mat[0].x = scale.x;
    mat[1].y = scale.y;
    mat[2].z = scale.z;
    mat[3].w = 1.0;
    return mat;
}

const vec3 worldAxis[3] = {
    vec3(1.0, 0.0, 0.0), // x-axis
    vec3(0.0, 1.0, 0.0), // y-axis
    vec3(0.0, 0.0, 1.0), // z-axis
};

const vec2 vertexPos[4] = {
    vec2(-1.0, -1.0),
    vec2( 1.0, -1.0),
    vec2(-1.0,  1.0),
    vec2( 1.0,  1.0),
};

float hash(float n) {
    return mod(fract(sin(n) * 43758.5453), 3.1415);
}

vec2 hash2(float n) {
    return vec2(hash(n), hash(n + 1.0));
}

void main() {
    const vec4 vertex = vec4(vertexPos[gl_VertexIndex], 0.0, 1.0);

    const vec2 noiseCoord = vec2(gl_InstanceIndex % NOISEMAP_SIZE, gl_InstanceIndex / NOISEMAP_SIZE) / textureSize(NoiseTex, 0);
    const float noise = texture(NoiseTex, noiseCoord).r;

    const vec3 starAxis = worldAxis[gl_InstanceIndex / 2];
    const vec3 distanceFromCamera = vec3(0.0, 0.0, 100.0);

    const mat4 model = rotate(noise, starAxis)
                       * scale(vec3(0.1))
                       * rotate(hash(noise) * 3.1415, vec3(0.0, 1.0, 0.0))
                       * rotate(hash(noise) * 2.0 * 3.1415, vec3(1.0, 0.0, 0.0))
                       * translate(distanceFromCamera);
    gl_Position = viewProj.proj * mat4(mat3(viewProj.view)) * model * vertex;
}