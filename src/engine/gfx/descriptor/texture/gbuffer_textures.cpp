/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gbuffer_textures.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 00:40:50 by etran             #+#    #+#             */
/*   Updated: 2024/06/21 14:31:45 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gbuffer_textures.h"
#include "device.h"
#include "swap_chain.h"
#include "buffer.h"
#include "icommand_buffer.h"

#include "debug.h"

#include <stdexcept>
#include <array>
#include <random>

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
    LDEBUG("Pos texture created.");
}

void PositionTexture::destroy(const Device& device) {
    m_imageBuffer.destroy(device);
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

    LDEBUG("Normal texture created.");
}

void NormalTexture::destroy(const Device& device) {
    m_imageBuffer.destroy(device);
}

void NormalTexture::fill(
    const Device& device,
    const ICommandBuffer* cmdBuffer,
    const void* data
) {
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
    LDEBUG("Albedo texture created.");
}

void AlbedoTexture::destroy(const Device& device) {
    m_imageBuffer.destroy(device);
}

void AlbedoTexture::fill(
    const Device& device,
    const ICommandBuffer* cmdBuffer,
    const void* data
) {
}

/* SSAO TEXTURE ============================================================= */

void SSAOTexture::init(const Device& device) {
    ImageMetaData textureData{};
    textureData.m_format = VK_FORMAT_R8_UNORM;
    textureData.m_width = SwapChain::getImageExtent().width;
    textureData.m_height = SwapChain::getImageExtent().height;
    textureData.m_sampleCount = VK_SAMPLE_COUNT_1_BIT;
    textureData.m_usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
                          VK_IMAGE_USAGE_SAMPLED_BIT;
    textureData.m_aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;

    m_imageBuffer.initImage(device, std::move(textureData));
    m_imageBuffer.initView(device);
    LDEBUG("SSAO texture created.");
}

void SSAOTexture::destroy(const Device& device) {
    m_imageBuffer.destroy(device);
}

void SSAOTexture::fill(
    const Device& device,
    const ICommandBuffer* cmdBuffer,
    const void* data
) {
}

} // namespace vox::gfx