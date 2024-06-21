/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadowmap_texture.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 16:13:11 by etran             #+#    #+#             */
/*   Updated: 2024/06/21 14:32:56 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shadowmap_texture.h"
#include "device.h"
#include "buffer.h"
#include "icommand_buffer.h"
#include "swap_chain.h"

namespace vox::gfx {

static constexpr u32 TEXTURE_SIZE = 4096;

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void ShadowmapSampler::init(const Device& device) {
    ImageMetaData depthImageMetaData{};
    depthImageMetaData.m_format = SwapChain::getDepthFormat();
    depthImageMetaData.m_sampleCount = VK_SAMPLE_COUNT_1_BIT;
    depthImageMetaData.m_width = TEXTURE_SIZE;
    depthImageMetaData.m_height = TEXTURE_SIZE;
    depthImageMetaData.m_usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | // Depth attachment, used for shadow mapping
                                 VK_IMAGE_USAGE_SAMPLED_BIT;                   // Sampled texture
    depthImageMetaData.m_aspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT;

    m_imageBuffer.initImage(device, std::move(depthImageMetaData));
    m_imageBuffer.initView(device);
}

void ShadowmapSampler::destroy(const Device& device) {
    m_imageBuffer.destroy(device);
}

/* ========================================================================== */

void ShadowmapSampler::fill(
    const Device& device,
    const ICommandBuffer* cmdBuffer,
    const void* data
) {}

} // namespace vox::gfx