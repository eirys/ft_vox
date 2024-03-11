/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_texture_sampler.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 17:46:33 by etran             #+#    #+#             */
/*   Updated: 2024/03/12 00:17:03 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game_texture_sampler.h"
#include "device.h"
#include "perlin_noise.h"
#include "buffer.h"
#include "icommand_buffer.h"

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void GameTextureSampler::init(
    const Device& device,
    const ICommandBuffer* cmdBuffer
) {
    ImageMetaData textureData{};
    textureData.m_format = VK_FORMAT_R8G8B8_SRGB;
    textureData.m_width = 100;
    textureData.m_height = 100;
    textureData.m_layerCount = 2;
    textureData.m_usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    textureData.m_viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
    textureData.computeMipCount();
    m_imageBuffer.initImage(device, std::move(textureData));

    // Placeholders: noise maps
    proc::NoiseMapInfo info{};
    info.type = proc::PerlinNoiseType::PERLIN_NOISE_2D;
    info.width = m_imageBuffer.getMetaData().m_width;
    info.height = m_imageBuffer.getMetaData().m_height;
    info.layers = 4;
    info.frequency_0 = 0.1f;
    info.frequency_mult = 2.0f;
    info.amplitude_mult = 0.5f;

    static std::array<proc::PerlinNoise, 2> noises = {
        proc::PerlinNoise(info),
        proc::PerlinNoise(info)
    };

    std::vector<u32>    pixels;
    pixels.resize(info.width * info.height * 2);
    noises[0].copyToBuffer(pixels.data());
    noises[1].copyToBuffer(pixels.data() + (info.width * info.height));

    BufferMetadata  bufferData{};
    bufferData.m_size = pixels.size() * sizeof(u32);
    bufferData.m_usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    bufferData.m_properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    Buffer stagingBuffer;
    stagingBuffer.init(device, std::move(bufferData));
    stagingBuffer.map(device);
    stagingBuffer.copyFrom(pixels.data());
    stagingBuffer.unmap(device);

    cmdBuffer->startRecording();
    m_imageBuffer.copyFrom(cmdBuffer, stagingBuffer);
    m_imageBuffer.generateMipmap(cmdBuffer);
    cmdBuffer->stopRecording();
    stagingBuffer.destroy(device);

    m_imageBuffer.initView(device);
    _createSampler(device);
}

void GameTextureSampler::destroy(const Device& device) {
    m_imageBuffer.destroy(device);
    vkDestroySampler(device.getDevice(), m_sampler, nullptr);
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

void GameTextureSampler::_createSampler(const Device& device) {
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = device.queryDeviceProperties().limits.maxSamplerAnisotropy;
    samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = (float)m_imageBuffer.getMetaData().m_mipCount;

    if (vkCreateSampler(device.getDevice(), &samplerInfo, nullptr, &m_sampler) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture sampler!");
    }
}


} // namespace vox::gfx