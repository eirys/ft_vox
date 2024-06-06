#version 450

#include "../src/engine/gfx/descriptor/sets/descriptor_decl.h"

layout(location = 0) out vec3 outUVW;

layout(set = PFD_SET, binding = 0) uniform Camera {
    mat4 view;
    mat4 proj;
} camera;

#define CORNER_A vec3( 1.0, -1.0,  1.0)
#define CORNER_B vec3( 1.0, -1.0, -1.0)
#define CORNER_C vec3( 1.0,  1.0, -1.0)
#define CORNER_D vec3( 1.0,  1.0,  1.0)
#define CORNER_E vec3(-1.0, -1.0,  1.0)
#define CORNER_F vec3(-1.0, -1.0, -1.0)
#define CORNER_G vec3(-1.0,  1.0, -1.0)
#define CORNER_H vec3(-1.0,  1.0,  1.0)

const vec3 QUAD_CORNER[6][4] = {
    { CORNER_A, CORNER_D, CORNER_B, CORNER_C }, // Face +X
    { CORNER_E, CORNER_F, CORNER_H, CORNER_G }, // Face -X
    { CORNER_C, CORNER_D, CORNER_G, CORNER_H }, // Face +Y
    { CORNER_A, CORNER_B, CORNER_E, CORNER_F }, // Face -Y
    { CORNER_E, CORNER_H, CORNER_A, CORNER_D }, // Face +Z
    { CORNER_B, CORNER_C, CORNER_F, CORNER_G }, // Face -Z
};

void main() {
    outUVW = QUAD_CORNER[gl_InstanceIndex][gl_VertexIndex];

    gl_Position = camera.proj * mat4(mat3(camera.view)) * vec4(outUVW, 1.0);
}