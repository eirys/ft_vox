/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skybox_texture.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 15:27:49 by etran             #+#    #+#             */
/*   Updated: 2024/06/21 14:33:27 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "skybox_texture.h"
#include "device.h"
#include "icommand_buffer.h"
#include "buffer.h"
#include "ppm_loader.h"
#include "debug.h"

namespace vox::gfx {

static constexpr u32 TEXTURE_SIZE = 16;

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void SkyboxSampler::init(const Device& device) {
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
}

void SkyboxSampler::destroy(const Device& device) {
    m_imageBuffer.destroy(device);
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

} // namespace vox::gfx