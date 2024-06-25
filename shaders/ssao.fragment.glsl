#version 450
#define VOX_SSAO_LAYOUT

#include "../src/engine/gfx/descriptor/sets/descriptor_decl.h"
#include "../src/engine/game/game_decl.h"

layout(location = 0) in vec2 inUV;

layout(location = 0) out float outFragColor;

layout(set = SSAO_SET, binding = 0) uniform Samples {
    vec3 at[SSAO_KERNEL_SIZE];
} samples;
layout(set = SSAO_SET, binding = 1) uniform sampler2DArray NoiseTex;
layout(set = SSAO_SET, binding = 2) uniform sampler2D PositionViewTex;
layout(set = SSAO_SET, binding = 3) uniform sampler2D NormalViewInv;

layout(push_constant) uniform Camera {
    mat4 view;
    mat4 proj;
} camera;

// --------------------------

const float BIAS = 0.05;
const float RADIUS = 1.0;

// --------------------------

void main() {
    // Scale UV to match noise texture
    const ivec2 posTexDim = textureSize(PositionViewTex, 0);
    const ivec2 noiseTexDim = textureSize(NoiseTex, 0).xy;
    const vec3 scale = vec3(
        float(posTexDim.x) / float(noiseTexDim.x),
        float(posTexDim.y) / float(noiseTexDim.y),
        1.0);

    // Retrieve from GBuffer
    const vec3 fragPos = texture(PositionViewTex, inUV).xyz;
    const vec3 normal = texture(NormalViewInv, inUV).xyz;
    const vec3 randVec = vec3(texture(NoiseTex, vec3(inUV, 0.0) * scale).xy, 0.0);

    // Create TBN matrix
    const vec3 tangent = normalize(randVec - normal * dot(randVec, normal));
    const vec3 bitangent = cross(tangent, normal);
    const mat3 TBN = mat3(tangent, bitangent, normal);

    float occlusion = 0.0;

    for (uint i = 0; i < SSAO_KERNEL_SIZE; ++i) {
        // Get sample position
        vec3 samplePos = TBN * samples.at[i];
        samplePos = fragPos + samplePos * RADIUS;

        // Project sample position
        vec4 offset = vec4(samplePos, 1.0);
        offset = camera.proj * offset;
        offset.xy /= offset.w;
        offset.xy = offset.xy * 0.5 + 0.5;

        // Compare depth values
        const float sampleDepth = texture(PositionViewTex, offset.xy).z;
        const float rangeCheck = smoothstep(0.0, 1.0, RADIUS / abs(fragPos.z - sampleDepth));
        occlusion += step(samplePos.z + BIAS, sampleDepth) * rangeCheck;
        // if (sampleDepth > samplePos.z + BIAS) occlusion += 1.0 * rangeCheck;
    }

    occlusion = 1.0 - (occlusion / float(SSAO_KERNEL_SIZE));
    outFragColor = occlusion;
}