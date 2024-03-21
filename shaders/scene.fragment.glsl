#version 450

layout(location = 0) in vec2 inUV;

layout(location = 0) out vec4 fragColor;

layout(set = 1, binding = 1) uniform sampler2DArray GameTex;

void main() {
    if (gl_FrontFacing)
        fragColor = texture(GameTex, vec3(inUV, 2));
    else
        fragColor = texture(GameTex, vec3(inUV, 0));
}