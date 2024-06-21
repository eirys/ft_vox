/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_noise_texture.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 15:41:25 by etran             #+#    #+#             */
/*   Updated: 2024/06/20 16:11:08 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "perlin_noise_texture.h"
#include "device.h"
#include "perlin_noise.h"
#include "icommand_buffer.h"
#include "buffer.h"
#include "game_decl.h"

#include <array>

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void PerlinNoiseSampler::init(const Device& device) {
    ImageMetaData textureData{};
    textureData.m_format = VK_FORMAT_R8G8B8A8_SRGB;
    textureData.m_width = NOISEMAP_SIZE;
    textureData.m_height = NOISEMAP_SIZE;
    textureData.m_usage = VK_IMAGE_USAGE_SAMPLED_BIT |      // Sampled texture
                          VK_IMAGE_USAGE_TRANSFER_DST_BIT;  // Transfer destination
    textureData.m_layerCount = 4;
    textureData.m_viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
    m_imageBuffer.initImage(device, std::move(textureData));
}

void PerlinNoiseSampler::fill(
    const Device& device,
    const ICommandBuffer* cmdBuffer,
    const void* data
) {
    proc::NoiseMapInfo info{};
    info.type = proc::PerlinNoiseType::PERLIN_NOISE_2D;
    info.width = m_imageBuffer.getMetaData().m_width;
    info.height = m_imageBuffer.getMetaData().m_height;
    info.layers = 2;
    info.frequency_0 = 2.0f;
    info.frequency_mult = 0.1f;
    info.amplitude_mult = 0.5f;

    info.seed = 16;
    const proc::PerlinNoise noise1(info);
    auto noiseData1 = noise1.toPixels();

    info.seed = 24;
    const proc::PerlinNoise noise2(info);
    auto noiseData2 = noise2.toPixels();

    info.seed = 32;
    const proc::PerlinNoise noise3(info);
    auto noiseData3 = noise3.toPixels();

    info.seed = 40;
    const proc::PerlinNoise noise4(info);
    auto noiseData4 = noise4.toPixels();

    const u32 imageSize = m_imageBuffer.getMetaData().getLayerSize()
                           * m_imageBuffer.getMetaData().getPixelSize();

    Buffer stagingBuffer = m_imageBuffer.createStagingBuffer(device);
    stagingBuffer.map(device);
    stagingBuffer.copyFrom(noiseData1.data(), imageSize, 0);
    stagingBuffer.copyFrom(noiseData2.data(), imageSize, imageSize);
    stagingBuffer.copyFrom(noiseData3.data(), imageSize, imageSize * 2);
    stagingBuffer.copyFrom(noiseData4.data(), imageSize, imageSize * 3);
    stagingBuffer.unmap(device);

    constexpr LayoutData FINAL_LAYOUT{
        .m_layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        .m_accessMask = VK_ACCESS_SHADER_READ_BIT,
        .m_stageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT };

    cmdBuffer->reset();
    cmdBuffer->startRecording();
    m_imageBuffer.copyFrom(cmdBuffer, stagingBuffer);
    m_imageBuffer.setLayout(cmdBuffer, FINAL_LAYOUT);
    cmdBuffer->stopRecording();
    cmdBuffer->awaitEndOfRecording(device);
    stagingBuffer.destroy(device);

    m_imageBuffer.initView(device);
}

void PerlinNoiseSampler::destroy(const Device& device) {
    m_imageBuffer.destroy(device);
}

} // namespace vox::gfx