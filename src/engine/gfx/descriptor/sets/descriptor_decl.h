/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_decl.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 23:42:29 by etran             #+#    #+#             */
/*   Updated: 2024/06/21 03:45:48 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DESCRIPTOR_DECL_H
# define DESCRIPTOR_DECL_H

#ifdef VOX_CPP

#include <vulkan/vulkan.h>
#include "types.h"
#include "vox_decl.h"

namespace vox::gfx {

/* ========================================================================== */
/*                                    ENUMS                                   */
/* ========================================================================== */

/**
 * @brief List of all descriptor sets.
 * @note The list is stored in the DescriptorTable class.
*/
enum class DescriptorSetIndex: u32 {
    Pfd,
    WorldData,
    GBuffer,
#if ENABLE_SSAO
    Ssao,
    SsaoBlur,
#endif

    Count
};

constexpr u32 DESCRIPTOR_TABLE_SIZE = (u32)DescriptorSetIndex::Count;

/* ========================================================================== */

enum class DescriptorTypeIndex: u8 {
    UniformBuffer,
    CombinedImageSampler,
    StorageBuffer,
    StorageImage,
    SampledImage,

    First = UniformBuffer,
    Last = SampledImage
};

constexpr u32 DESCRIPTOR_TYPE_COUNT = enumSize<DescriptorTypeIndex>();

/**
 * @brief Every descriptor type handled in the engine.
*/
constexpr VkDescriptorType DESCRIPTOR_TYPES[DESCRIPTOR_TYPE_COUNT] = {
    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
    VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
    VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
    VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
    VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE
};

/* ========================================================================== */

/**
 * @brief Shader visibility for the descriptor.
*/
enum class ShaderVisibility: u8 {
    VS = VK_SHADER_STAGE_VERTEX_BIT,
    FS = VK_SHADER_STAGE_FRAGMENT_BIT,
    CS = VK_SHADER_STAGE_COMPUTE_BIT,
    VS_FS = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
};

inline
ShaderVisibility operator|(ShaderVisibility lhs, ShaderVisibility rhs) {
    return static_cast<ShaderVisibility>(static_cast<u8>(lhs) | static_cast<u8>(rhs));
}

} // namespace vox::gfx

#else

#if defined(VOX_SCENE_LAYOUT)
# define PFD_SET 0
# define GBUFFER_SET 1

#elif defined(VOX_SKY_LAYOUT)
#define PFD_SET 0
#define WORLD_SET 1

#elif defined(VOX_DEFERRED_LAYOUT)
#define WORLD_SET 0

#elif defined(VOX_SHADOW_LAYOUT)
#define PFD_SET 0

#elif defined(VOX_SSAO_LAYOUT)
#define SSAO_SET 0

#elif defined(VOX_SSAO_BLUR_LAYOUT)
#define SSAO_BLUR_SET 0

#endif

#endif // VOX_CPP
#endif // DESCRIPTOR_DECL_H