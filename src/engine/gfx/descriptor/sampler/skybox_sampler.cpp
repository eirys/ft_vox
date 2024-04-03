/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skybox_sampler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 15:27:49 by etran             #+#    #+#             */
/*   Updated: 2024/04/03 00:23:08 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "skybox_sampler.h"
#include "device.h"
#include "icommand_buffer.h"
#include "buffer.h"
#include "debug.h"

#include "ppm_loader.h"

namespace vox::gfx {

static constexpr u32 TEXTURE_SIZE = 16;
static constexpr u32 TEXTURE_COUNT = 1;

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void SkyboxSampler::init(
    const Device& device,
    const ICommandBuffer* cmdBuffer
) {
    ImageMetaData textureData{};
    textureData.m_format = VK_FORMAT_R8G8B8A8_SRGB;
    textureData.m_width = TEXTURE_SIZE;
    textureData.m_height = TEXTURE_SIZE;
    textureData.m_layerCount = 6; // Cubemap
    textureData.m_usage = VK_IMAGE_USAGE_SAMPLED_BIT |
                          VK_IMAGE_USAGE_TRANSFER_SRC_BIT | // For mipmap generation
                          VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    textureData.m_viewType = VK_IMAGE_VIEW_TYPE_CUBE;
    textureData.m_flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
    textureData.computeMipCount();
    m_imageBuffer.initImage(device, std::move(textureData));
    _createSampler(device);
}

void SkyboxSampler::destroy(const Device& device) {
    m_imageBuffer.destroy(device);
    vkDestroySampler(device.getDevice(), m_sampler, nullptr);
}

/* ========================================================================== */

static
scop::Image _loadSkybox() {
    const std::string texturePath = "assets/textures/sky.ppm";
    scop::PpmLoader loader(texturePath);

    return loader.load();
}

void SkyboxSampler::fill(
    const Device& device,
    const ICommandBuffer* cmdBuffer,
    const void* data
) {
    const u32 IMAGE_SIZE = m_imageBuffer.getMetaData().getLayerSize()
                           * m_imageBuffer.getMetaData().getPixelSize();

    scop::Image texture = _loadSkybox();

    Buffer stagingBuffer = m_imageBuffer.createStagingBuffer(device);
    stagingBuffer.map(device);
    for (u32 i = 0; i < m_imageBuffer.getMetaData().m_layerCount; ++i)
        stagingBuffer.copyFrom(texture.getPixels(), IMAGE_SIZE, i * IMAGE_SIZE);
    stagingBuffer.unmap(device);

    cmdBuffer->reset();
    cmdBuffer->startRecording();
    m_imageBuffer.copyFrom(cmdBuffer, stagingBuffer);
    m_imageBuffer.generateMipmap(cmdBuffer);
    cmdBuffer->stopRecording();
    cmdBuffer->awaitEndOfRecording(device);
    stagingBuffer.destroy(device);

    m_imageBuffer.initView(device);
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

void SkyboxSampler::_createSampler(const Device& device) {
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = device.queryDeviceProperties().limits.maxSamplerAnisotropy;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_NEVER;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = (float)m_imageBuffer.getMetaData().m_mipCount;
    samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;

    if (vkCreateSampler(device.getDevice(), &samplerInfo, nullptr, &m_sampler) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture sampler!");
    }
}

} // namespace vox::gfx