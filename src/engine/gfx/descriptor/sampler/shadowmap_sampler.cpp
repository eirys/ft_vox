/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadowmap_sampler.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 16:13:11 by etran             #+#    #+#             */
/*   Updated: 2024/06/03 18:59:32 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shadowmap_sampler.h"
#include "device.h"
#include "buffer.h"
#include "icommand_buffer.h"

#include "ppm_loader.h"

namespace vox::gfx {

static constexpr u32 TEXTURE_SIZE = 1024;

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void ShadowmapSampler::init(const Device& device) {
    ImageMetaData depthImageMetaData{};
    depthImageMetaData.m_format = VK_FORMAT_D16_UNORM;
    depthImageMetaData.m_sampleCount = VK_SAMPLE_COUNT_1_BIT;
    depthImageMetaData.m_width = TEXTURE_SIZE;
    depthImageMetaData.m_height = TEXTURE_SIZE;
    depthImageMetaData.m_usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | // Depth attachment, used for shadow mapping
                                 VK_IMAGE_USAGE_SAMPLED_BIT;                   // Sampled texture
    depthImageMetaData.m_aspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT;

    m_imageBuffer.initImage(device, std::move(depthImageMetaData));
    m_imageBuffer.initView(device);

    _createSampler(device);
}

void ShadowmapSampler::destroy(const Device& device) {
    m_imageBuffer.destroy(device);
    vkDestroySampler(device.getDevice(), m_sampler, nullptr);
}

/* ========================================================================== */

void ShadowmapSampler::fill(
    const Device& device,
    const ICommandBuffer* cmdBuffer,
    const void* data
) {}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

void ShadowmapSampler::_createSampler(const Device& device) {
    VkSamplerCreateInfo samplerInfo{};

    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    samplerInfo.anisotropyEnable = VK_FALSE;
    samplerInfo.maxAnisotropy = 0.0f;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_NEVER;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 1.0f;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;

    if (vkCreateSampler(device.getDevice(), &samplerInfo, nullptr, &m_sampler) != VK_SUCCESS)
        throw std::runtime_error("failed to create shadow sampler");
}

} // namespace vox::gfx