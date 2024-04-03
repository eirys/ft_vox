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

layout(set = WORLD_SET, binding = 2) uniform sampler2DArray NoiseTex;

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

// Generate rotation matrix from cos and sin input
mat4 rotate2(in vec2 cs, in vec3 axis) {
    mat4 mat = mat4(1.0);
    const float oc = 1.0 - cs.x;
    const vec3 naxis = normalize(axis);

    mat *= mat4(
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
    return mat;
}

// Generate a translation matrix from a direction
mat4 translate(in vec3 dir) {
    mat4 mat = mat4(1.0);
    mat[3].xyz = dir;
    return mat;
}

#define X_AXIS vec3(1.0, 0.0, 0.0)
#define Y_AXIS vec3(0.0, 1.0, 0.0)
#define Z_AXIS vec3(0.0, 0.0, 1.0)
#define PI 3.141592

const vec3 vertexPos[4] = {
    vec3(-1.0, -1.0, 0.0),
    vec3( 1.0, -1.0, 0.0),
    vec3(-1.0,  1.0, 0.0),
    vec3( 1.0,  1.0, 0.0),
};
const float starSize = 0.1;
const vec3 skyDist = vec3(0.0, 0.0, 150.0);

void main() {
    const vec2 noiseCoord = vec2(gl_InstanceIndex % NOISEMAP_SIZE, gl_InstanceIndex / NOISEMAP_SIZE) / textureSize(NoiseTex, 0).xy;
    const float random1 = texture(NoiseTex, vec3(noiseCoord, 0)).r;
    const float random2 = texture(NoiseTex, vec3(noiseCoord, 1)).r;

    const vec2 moonDir = vec2(gameData.sunPos.x, -gameData.sunPos.y);

    const mat4 model = rotate(random1 * 2.0 *  PI, X_AXIS)             // Rotate star for variety
                    //    * rotate2(moonDir, Z_AXIS)               // Rotate star to face the sun
                    //    * rotate(random2 * 2.0, X_AXIS)     // Displace star randomly
                    //    * rotate(random2 * 2.0, Z_AXIS)     // Displace star randomly
                    //    * rotate(random2 * PI * 2.0, Y_AXIS)     // Displace star randomly
                       * translate(skyDist)                     // Move star to the sky
                       ;

    const vec4 vertex = vec4(vertexPos[gl_VertexIndex], 1.0);
    gl_Position = viewProj.proj * mat4(mat3(viewProj.view)) * model * vertex;
}