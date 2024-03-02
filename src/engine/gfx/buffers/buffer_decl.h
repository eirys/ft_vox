/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer_decl.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 00:07:00 by etran             #+#    #+#             */
/*   Updated: 2024/03/01 00:10:01 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vulkan/vulkan.h>

#include "types.h"
#include "enum.h"

namespace vox::gfx {

enum class BlittedImageBuffers: u32 {
    Textures = 0,

    First = Textures,
    Last = Textures
};

constexpr u32       BLITTED_IMAGE_COUNT = enumSize<BlittedImageBuffers>();

constexpr VkFormat  BLITTED_IMAGE_FORMATS[BLITTED_IMAGE_COUNT] = {
    VK_FORMAT_R8G8B8A8_SRGB // Textures
};

} // namespace vox::gfx
