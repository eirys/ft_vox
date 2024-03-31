#version 450

#include "../src/engine/gfx/descriptor/sets/descriptor_decl.h"
#include "../src/engine/game/game_decl.h"

// 1 quad = 1 noise value:
// generate rotation matrix
// generate translation matrix
// 1 quad = 1 color

layout(set = WORLD_SET, binding = 2) uniform sampler2D NoiseTex;

const vec2 vertexPos[4] = {
    { 0.0, 0.0 },
    { 1.0, 0.0 },
    { 0.0, 1.0 },
    { 1.0, 1.0 },
};

void rotate(
    inout mat4 mat,
    in float angle,
    in vec3 axis
) {
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

        0.0,
        0.0,
        0.0,
        1.0
    );
}

void translate(
    inout mat4 mat,
    in vec3 dir
) {
    for (uint i = 0; i < 4; ++i) {
        const float res = (mat[i].x * dir.x) + (mat[i].y * dir.y) + (mat[i].z * dir.z) + mat[i].w;
        mat[i] = vec4(mat[i].x, mat[i].y, mat[i].z, res);
    }
}

#define CORNER_A vec3( 1.0, -1.0,  1.0)
#define CORNER_B vec3( 1.0, -1.0, -1.0)
#define CORNER_C vec3( 1.0,  1.0, -1.0)
#define CORNER_D vec3( 1.0,  1.0,  1.0)
#define CORNER_E vec3(-1.0, -1.0,  1.0)
#define CORNER_F vec3(-1.0, -1.0, -1.0)
#define CORNER_G vec3(-1.0,  1.0, -1.0)
#define CORNER_H vec3(-1.0,  1.0,  1.0)

const vec3 quadCorner[6][4] = {
    { CORNER_A, CORNER_D, CORNER_B, CORNER_C }, // Face +X
    { CORNER_E, CORNER_F, CORNER_H, CORNER_G }, // Face -X
    { CORNER_C, CORNER_D, CORNER_G, CORNER_H }, // Face +Y
    { CORNER_A, CORNER_B, CORNER_E, CORNER_F }, // Face -Y
    { CORNER_E, CORNER_H, CORNER_A, CORNER_D }, // Face +Z
    { CORNER_B, CORNER_C, CORNER_F, CORNER_G }, // Face -Z
};

void main() {
    vec2 vertex = vertexPos[gl_VertexIndex];

    vec2 noiseCoord = vec2(gl_InstanceIndex % NOISEMAP_SIZE, gl_InstanceIndex / NOISEMAP_SIZE) / textureSize(NoiseTex, 0);
    float noise = texture(NoiseTex, noiseCoord).r;

    mat4 model = mat4(1.0);
    rotate(model, noise, vec3(0.0, 0.0, 1.0));
    translate(model, vec3(noise, -noise, 2 * noise));
}