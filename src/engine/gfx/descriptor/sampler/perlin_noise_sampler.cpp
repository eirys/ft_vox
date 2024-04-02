/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_noise_sampler.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 15:41:25 by etran             #+#    #+#             */
/*   Updated: 2024/04/02 02:35:19 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "perlin_noise_sampler.h"
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

void PerlinNoiseSampler::init(const Device& device, const ICommandBuffer* cmdBuffer) {
    ImageMetaData textureData{};
    textureData.m_format = VK_FORMAT_R8G8B8A8_SRGB;
    textureData.m_width = NOISEMAP_SIZE;
    textureData.m_height = NOISEMAP_SIZE;
    textureData.m_usage = VK_IMAGE_USAGE_SAMPLED_BIT |      // Sampled texture
                          VK_IMAGE_USAGE_TRANSFER_DST_BIT;  // Transfer destination
    m_imageBuffer.initImage(device, std::move(textureData));
    _createSampler(device);
}

void PerlinNoiseSampler::fill(
    const Device& device,
    const ICommandBuffer* cmdBuffer,
    const void* data
) {
    constexpr LayoutData finalLayout{
        .m_layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        .m_accessMask = VK_ACCESS_SHADER_READ_BIT,
        .m_stageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT };

    proc::NoiseMapInfo info{};
    info.seed = 16;
    info.type = proc::PerlinNoiseType::PERLIN_NOISE_2D;
    info.width = m_imageBuffer.getMetaData().m_width;
    info.height = m_imageBuffer.getMetaData().m_height;
    info.layers = 1;
    info.frequency_0 = 1.0f;//0.03f;
    info.frequency_mult = 3.0f;
    info.amplitude_mult = 0.5f;

    const proc::PerlinNoise noise(info);
    auto noiseData = noise.toPixels();

    Buffer stagingBuffer = m_imageBuffer.createStagingBuffer(device);
    stagingBuffer.map(device);
    stagingBuffer.copyFrom(noiseData.data());
    stagingBuffer.unmap(device);

    cmdBuffer->reset();
    cmdBuffer->startRecording();
    m_imageBuffer.copyFrom(cmdBuffer, stagingBuffer);
    m_imageBuffer.setLayout(cmdBuffer, finalLayout);
    cmdBuffer->stopRecording();
    cmdBuffer->awaitEndOfRecording(device);
    stagingBuffer.destroy(device);

    m_imageBuffer.initView(device);
}

void PerlinNoiseSampler::destroy(const Device& device) {
    m_imageBuffer.destroy(device);
    vkDestroySampler(device.getDevice(), m_sampler, nullptr);
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

void PerlinNoiseSampler::_createSampler(const Device& device) {
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_NEAREST;
    samplerInfo.minFilter = VK_FILTER_NEAREST;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_FALSE;
    samplerInfo.maxAnisotropy = 1.0;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_NEVER;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 1.0f;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;

    if (vkCreateSampler(device.getDevice(), &samplerInfo, nullptr, &m_sampler) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture sampler!");
    }
}

} // namespace vox::gfx