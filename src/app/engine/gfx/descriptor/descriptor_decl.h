/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_decl.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 23:42:29 by etran             #+#    #+#             */
/*   Updated: 2024/02/28 23:43:56 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vulkan/vulkan.h>

#include "types.h"

namespace vox::gfx {

/* ========================================================================== */
/*                                    ENUMS                                   */
/* ========================================================================== */

/**
 * @brief Descriptor set index.
*/
enum class DescriptorIndex: u32 {
    Mvp = 0, // placeholder

    First = Mvp,
    Last = Mvp
};

constexpr u32 DESCRIPTOR_TABLE_SIZE = enumSize<DescriptorIndex>();

/* ========================================================================== */

enum class DescriptorTypeIndex: u8 {
    UniformBuffer = 0,
    CombinedImageSampler,
    StorageBuffer,
    StorageImage,
    SampledImage,

    First = UniformBuffer,
    Last = SampledImage
};

constexpr u32 DESCRIPTOR_TYPE_COUNT = enumSize<DescriptorTypeIndex>();

constexpr VkDescriptorType DESCRIPTOR_TYPES[DESCRIPTOR_TYPE_COUNT] = {
    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
    VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
    VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
    VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
    VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE
};

// enum class DescriptorType: u8 {
//     UniformBuffer           = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
//     CombinedImageSampler    = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
//     StorageBuffer           = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
//     StorageImage            = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
//     SampledImage            = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE
// };

enum class ShaderVisibility: u8 {
    VS = VK_SHADER_STAGE_VERTEX_BIT,
    FS = VK_SHADER_STAGE_FRAGMENT_BIT,
    CS = VK_SHADER_STAGE_COMPUTE_BIT
};

} // namespace vox::gfx
