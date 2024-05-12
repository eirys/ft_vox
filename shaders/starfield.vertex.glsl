#version 450

#include "../src/engine/gfx/descriptor/sets/descriptor_decl.h"
#include "../src/engine/game/game_decl.h"

layout(location = 0) out float outIntensity;

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

const vec3 vertexPos[4] = {
    vec3(-1.0, -1.0, 0.0), // r
    vec3( 1.0, -1.0, 0.0), // g
    vec3(-1.0,  1.0, 0.0), // b
    vec3( 1.0,  1.0, 0.0), // w
};

const vec3 vertexColor[4] = {
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0),
    vec3(1.0)
};

const vec3 X_AXIS = vec3(1.0, 0.0, 0.0);
const vec3 Y_AXIS = vec3(0.0, 1.0, 0.0);
const vec3 Z_AXIS = vec3(0.0, 0.0, 1.0);
const vec3 skyDist = vec3(0.0, 0.0, 150.0);
const float PI = 3.141592;

vec3 getWorldAxis(in mat4 inverseModel, in vec3 localAxis) {
    return (inverseModel * vec4(localAxis, 0.0)).xyz;
}

mat4 getModel(in vec3 random) {
    const vec3 moonDir = vec3(gameData.sunPos.x, -gameData.sunPos.y, 0.0);

    // Angle of the star
    const mat4 starAngle = rotate(random.x * PI * 2.0, Z_AXIS);

    // Rotation of the star along world Y axis
    const mat4 starYPlacement = rotate((random.y * 0.8 + 0.1) * PI, getWorldAxis(transpose(starAngle), Y_AXIS));

    // Rotation of the star along world Z axis
    const mat4 starZPlacement = rotate((random.z * 0.66 * PI - (0.33 * PI)), getWorldAxis(transpose(starYPlacement) * transpose(starAngle), Z_AXIS));

    // Angle of the star in the sky, facing the sun
    const mat4 starHeightInSky = rotate2(moonDir.xy, getWorldAxis(transpose(starZPlacement) * transpose(starYPlacement) * transpose(starAngle), Z_AXIS));

    // Move star to the sky
    const mat4 starDistance = translate(skyDist);

    return starAngle
         * starYPlacement
         * starZPlacement
         * starHeightInSky
         * starDistance;
}

void main() {
    const vec2 noiseCoord = vec2(gl_InstanceIndex % NOISEMAP_SIZE, gl_InstanceIndex / NOISEMAP_SIZE) / textureSize(NoiseTex, 0).xy;

    const vec3 random = vec3(
        texture(NoiseTex, vec3(noiseCoord, 0)).r,
        texture(NoiseTex, vec3(noiseCoord, 1)).r,
        texture(NoiseTex, vec3(noiseCoord, 2)).r);

    const mat4 model = getModel(random);
    const vec4 vertex = vec4(vertexPos[gl_VertexIndex], 1.0);
    gl_Position = viewProj.proj * mat4(mat3(viewProj.view)) * model * vertex;
}