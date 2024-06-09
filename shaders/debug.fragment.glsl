#version 450

#include "../src/engine/gfx/descriptor/sets/descriptor_decl.h"
#include "../src/engine/vox_decl.h"

layout(location = 0) in vec2 inUV;

layout(location = 0) out vec4 outColor;

#if ENABLE_SHADOW_MAPPING
    layout(set = PFD_SET, binding = 3) uniform sampler2D Shadowmap;
#endif

void main() {
    float depthValue = texture(Shadowmap, inUV).r;
    outColor = vec4(vec3(depthValue), 1.0);
}