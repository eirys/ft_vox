#version 450

#include "../src/engine/game/game_decl.h"
#include "../src/engine/gfx/descriptor/sets/descriptor_decl.h"
#include "../src/engine/vox_decl.h"

layout(location = 0) in vec3 inUVW;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inPosition;

layout(location = 0) out vec4 outPosition;
layout(location = 1) out vec4 outNormal;
layout(location = 2) out vec4 outAlbedo;

layout(set = WORLD_SET, binding = 0) uniform sampler2DArray GameTex;

void main() {
    outPosition = vec4(inPosition, gl_FragCoord.w);
    outNormal = vec4(inNormal, gl_FragCoord.z / gl_FragCoord.w);
    outAlbedo = texture(GameTex, inUVW);
}