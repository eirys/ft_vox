/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_texturess.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 17:46:33 by etran             #+#    #+#             */
/*   Updated: 2024/06/20 12:46:20 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game_textures.h"
#include "device.h"
#include "buffer.h"
#include "icommand_buffer.h"

#include "ppm_loader.h"

namespace vox::gfx {

static constexpr u32 IMAGE_SIZE = 16;
static constexpr u32 IMAGE_COUNT = 6;

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void GameTextureSampler::init(const Device& device) {
    ImageMetaData textureData{};
    textureData.m_format = VK_FORMAT_R8G8B8A8_SRGB;
    textureData.m_width = IMAGE_SIZE;
    textureData.m_height = IMAGE_SIZE;
    textureData.m_layerCount = IMAGE_COUNT;
    textureData.m_usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    textureData.m_viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
    textureData.computeMipCount();
    m_imageBuffer.initImage(device, std::move(textureData));
}

void GameTextureSampler::destroy(const Device& device) {
    m_imageBuffer.destroy(device);
}

/* ========================================================================== */

static
std::array<scop::Image, IMAGE_COUNT> _loadAssets() {
    std::array<scop::Image, IMAGE_COUNT> textures;

    std::array<std::string, IMAGE_COUNT> texturePaths = {
        "assets/textures/dirt.ppm",
        "assets/textures/grass_side.ppm",
        "assets/textures/grass_top.ppm",
        "assets/textures/stone.ppm",
        "assets/textures/sand.ppm",
        "assets/textures/undefined.ppm"
    };

    for (u32 i = 0; i < IMAGE_COUNT; ++i) {
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

    std::array<scop::Image, IMAGE_COUNT> textures = _loadAssets();

    Buffer stagingBuffer = m_imageBuffer.createStagingBuffer(device);
    stagingBuffer.map(device);
    for (u32 i = 0; i < IMAGE_COUNT; ++i)
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

} // namespace vox::gfx