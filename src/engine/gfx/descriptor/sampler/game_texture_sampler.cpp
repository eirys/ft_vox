/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_texture_sampler.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 17:46:33 by etran             #+#    #+#             */
/*   Updated: 2024/06/10 20:22:30 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game_texture_sampler.h"
#include "device.h"
#include "buffer.h"
#include "icommand_buffer.h"

#include "ppm_loader.h"

namespace vox::gfx {

static constexpr u32 TEXTURE_SIZE = 16;
static constexpr u32 TEXTURE_COUNT = 5;

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void GameTextureSampler::init(const Device& device) {
    ImageMetaData textureData{};
    textureData.m_format = VK_FORMAT_R8G8B8A8_SRGB;
    textureData.m_width = TEXTURE_SIZE;
    textureData.m_height = TEXTURE_SIZE;
    textureData.m_layerCount = TEXTURE_COUNT;
    textureData.m_usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    textureData.m_viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
    textureData.computeMipCount();
    m_imageBuffer.initImage(device, std::move(textureData));
    _createSampler(device);
}

void GameTextureSampler::destroy(const Device& device) {
    m_imageBuffer.destroy(device);
    vkDestroySampler(device.getDevice(), m_sampler, nullptr);
}

/* ========================================================================== */

static
std::array<scop::Image, TEXTURE_COUNT> _loadAssets() {
    std::array<scop::Image, TEXTURE_COUNT> textures;

    std::array<std::string, TEXTURE_COUNT> texturePaths = {
        "assets/textures/dirt.ppm",
        "assets/textures/grass_side.ppm",
        "assets/textures/grass_top.ppm",
        "assets/textures/stone.ppm",
        "assets/textures/sand.ppm"
    };

    for (u32 i = 0; i < TEXTURE_COUNT; ++i) {
        scop::PpmLoader loader(texturePaths[i]);
        textures[i] = loader.load();
    }

    return textures;
}

void GameTextureSampler::fill(
    const Device& device,
    const ICommandBuffer* cmdBuffer,
    const void* data
) {
    const u32 IMAGE_SIZE = m_imageBuffer.getMetaData().getLayerSize()
                           * m_imageBuffer.getMetaData().getPixelSize();

    std::array<scop::Image, TEXTURE_COUNT> textures = _loadAssets();

    Buffer stagingBuffer = m_imageBuffer.createStagingBuffer(device);
    stagingBuffer.map(device);
    for (u32 i = 0; i < TEXTURE_COUNT; ++i)
        stagingBuffer.copyFrom(textures[i].getPixels(), IMAGE_SIZE, i * IMAGE_SIZE);
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

void GameTextureSampler::_createSampler(const Device& device) {
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_NEAREST;
    samplerInfo.minFilter = VK_FILTER_NEAREST;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.anisotropyEnable = VK_FALSE;
    samplerInfo.maxAnisotropy = 1.0f;
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