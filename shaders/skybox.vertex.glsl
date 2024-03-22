#version 450

layout(location = 0) out vec3 outForward;

layout(set = 0, binding = 0) uniform ViewProj {
    mat4 view;
    mat4 proj;
} viewProj;

#define CORNER_A vec3( 1.0, -1.0,  1.0)
#define CORNER_B vec3( 1.0, -1.0, -1.0)
#define CORNER_C vec3( 1.0,  1.0, -1.0)
#define CORNER_D vec3( 1.0,  1.0,  1.0)
#define CORNER_E vec3(-1.0, -1.0,  1.0)
#define CORNER_F vec3(-1.0, -1.0, -1.0)
#define CORNER_G vec3(-1.0,  1.0, -1.0)
#define CORNER_H vec3(-1.0,  1.0,  1.0)

const vec3 quadCorner[6][4] = {
    // Face +X
    { CORNER_A, CORNER_D, CORNER_B, CORNER_C },
    // Face -X
    { CORNER_E, CORNER_F, CORNER_H, CORNER_G },
    // Face +Y
    { CORNER_C, CORNER_D, CORNER_G, CORNER_H },
    // Face -Y
    { CORNER_A, CORNER_B, CORNER_E, CORNER_F },
    // Face +Z
    { CORNER_A, CORNER_D, CORNER_E, CORNER_H },
    // Face -Z
    { CORNER_B, CORNER_C, CORNER_F, CORNER_G },
};

void main() {
    outForward = quadCorner[gl_InstanceIndex][gl_VertexIndex];
    gl_Position = viewProj.proj * mat4(mat3(viewProj.view)) * vec4(outForward, 1.0);
}