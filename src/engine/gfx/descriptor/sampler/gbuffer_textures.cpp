/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gbuffer_textures.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 00:40:50 by etran             #+#    #+#             */
/*   Updated: 2024/06/14 13:51:19 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gbuffer_textures.h"
#include "device.h"
#include "swap_chain.h"

#include <stdexcept>

namespace vox::gfx {

/* POSITION TEXTURE ========================================================= */

void PositionTexture::init(const Device& device) {
    ImageMetaData textureData{};
    textureData.m_format = VK_FORMAT_R16G16B16A16_SFLOAT;
    textureData.m_width = SwapChain::getImageExtent().width;
    textureData.m_height = SwapChain::getImageExtent().height;
    textureData.m_sampleCount = VK_SAMPLE_COUNT_1_BIT;
    textureData.m_usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
                          VK_IMAGE_USAGE_SAMPLED_BIT;
    textureData.m_aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;

    m_imageBuffer.initImage(device, std::move(textureData));
    m_imageBuffer.initView(device);

    VkSamplerCreateInfo samplerInfo{};

    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_NEAREST;
    samplerInfo.minFilter = VK_FILTER_NEAREST;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.maxAnisotropy = 1.0f;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 1.0f;
    samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

    if (vkCreateSampler(device.getDevice(), &samplerInfo, nullptr, &m_sampler) != VK_SUCCESS)
        throw std::runtime_error("failed to create shadow sampler");
}

void PositionTexture::destroy(const Device& device) {
    m_imageBuffer.destroy(device);
    vkDestroySampler(device.getDevice(), m_sampler, nullptr);
}

void PositionTexture::fill(
    const Device& device,
    const ICommandBuffer* cmdBuffer,
    const void* data
) {}

/* NORMAL TEXTURE =========================================================== */

void NormalTexture::init(const Device& device) {
    ImageMetaData textureData{};
    textureData.m_format = VK_FORMAT_R16G16B16A16_SFLOAT;
    textureData.m_width = SwapChain::getImageExtent().width;
    textureData.m_height = SwapChain::getImageExtent().height;
    textureData.m_sampleCount = VK_SAMPLE_COUNT_1_BIT;
    textureData.m_usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
                          VK_IMAGE_USAGE_SAMPLED_BIT;
    textureData.m_aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;

    m_imageBuffer.initImage(device, std::move(textureData));
    m_imageBuffer.initView(device);
}

void NormalTexture::destroy(const Device& device) {
    m_imageBuffer.destroy(device);
}

void NormalTexture::fill(
    const Device& device,
    const ICommandBuffer* cmdBuffer,
    const void* data
) {
    m_sampler = *(VkSampler*)data;
}

/* ALBEDO TEXTURE =========================================================== */

void AlbedoTexture::init(const Device& device) {
    ImageMetaData textureData{};
    textureData.m_format = VK_FORMAT_R8G8B8A8_UNORM;
    textureData.m_width = SwapChain::getImageExtent().width;
    textureData.m_height = SwapChain::getImageExtent().height;
    textureData.m_sampleCount = VK_SAMPLE_COUNT_1_BIT;
    textureData.m_usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
                          VK_IMAGE_USAGE_SAMPLED_BIT;
    textureData.m_aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;

    m_imageBuffer.initImage(device, std::move(textureData));
    m_imageBuffer.initView(device);
}

void AlbedoTexture::destroy(const Device& device) {
    m_imageBuffer.destroy(device);
}

void AlbedoTexture::fill(
    const Device& device,
    const ICommandBuffer* cmdBuffer,
    const void* data
) {
    m_sampler = *(VkSampler*)data;
}

} // namespace vox::gfx