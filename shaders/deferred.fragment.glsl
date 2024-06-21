#version 450
#define VOX_DEFERRED_LAYOUT

#include "../src/engine/game/game_decl.h"
#include "../src/engine/gfx/descriptor/sets/descriptor_decl.h"
#include "../src/engine/vox_decl.h"

layout(location = 0) in vec3 inUVW;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inPosition;
layout(location = 3) in vec3 inNormalView;

layout(location = 0) out vec4 outPosition;
layout(location = 1) out vec4 outNormal;
layout(location = 2) out vec4 outAlbedo;
layout(location = 3) out vec4 outNormalView;

layout(set = WORLD_SET, binding = 0) uniform sampler2DArray GameTex;

void main() {
    outPosition = vec4(inPosition, gl_FragCoord.z / gl_FragCoord.w);
    outNormal = vec4(inNormal, 1.0);
    outAlbedo = texture(GameTex, inUVW);
    outNormalView = vec4(normalize(inNormalView), 1.0);
}