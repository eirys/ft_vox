#version 450

#include "../src/engine/game/game_decl.h"

layout(location = 0) out vec3 outUVW;
layout(location = 1) out vec3 outSunDir;
// layout(location = 2) out vec3 outSunColor;

layout(set = 0, binding = 0) uniform ViewProj {
    mat4 view;
    mat4 proj;
} viewProj;

layout(set = 0, binding = 1) uniform GameData {
    vec2 sunPos;
    // uint sunColor;
} gameData;

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
    { CORNER_A, CORNER_D, CORNER_E, CORNER_H }, // Face +Z
    { CORNER_B, CORNER_C, CORNER_F, CORNER_G }, // Face -Z
};

void main() {
    outSunDir = vec3(gameData.sunPos, 0.0);
    // outSunColor = vec3((
    //     gameData.sunColor & 0xFF) / 255.0,
    //     ((gameData.sunColor >> 8) & 0xFF) / 255.0,
    //     ((gameData.sunColor >> 16) & 0xFF) / 255.0);

    // Skybox
    outUVW = quadCorner[gl_InstanceIndex][gl_VertexIndex];
    gl_Position = viewProj.proj * mat4(mat3(viewProj.view)) * vec4(outUVW, 1.0);
}