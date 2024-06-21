/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_data_sampler.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 23:05:38 by etran             #+#    #+#             */
/*   Updated: 2024/06/21 14:30:11 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chunk_data_texture.h"
#include "device.h"
#include "chunk.h"
#include "buffer.h"
#include "icommand_buffer.h"
#include "world.h"

#include <stdexcept>

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void ChunkDataSampler::init(const Device& device) {
    ImageMetaData textureData{};
    textureData.m_format = VK_FORMAT_R32_UINT;
    textureData.m_width = CHUNK_SIZE * 4;
    textureData.m_height = CHUNK_SIZE * 4;
    textureData.m_layerCount = RENDER_AREA; // A layer = a chunk
    textureData.m_usage = VK_IMAGE_USAGE_SAMPLED_BIT |      // Sampled texture
                          VK_IMAGE_USAGE_TRANSFER_DST_BIT;  // Transfer destination
    textureData.m_viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
    m_imageBuffer.initImage(device, std::move(textureData));
    // _createSampler(device);
}

void ChunkDataSampler::fill(
    const Device& device,
    const ICommandBuffer* cmdBuffer,
    const void* data
) {
    constexpr LayoutData finalLayout{
        .m_layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        .m_accessMask = VK_ACCESS_SHADER_READ_BIT,
        .m_stageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT };

    const auto& chunks = *(const game::World::ChunkArray*)data;

    Buffer stagingBuffer = m_imageBuffer.createStagingBuffer(device);
    stagingBuffer.map(device);
    for (u32 i = 0; i < RENDER_AREA; i++)
        stagingBuffer.copyFrom(chunks[i].getBlocks().data(), CHUNK_VOLUME, i * CHUNK_VOLUME);
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

void ChunkDataSampler::destroy(const Device& device) {
    m_imageBuffer.destroy(device);
    vkDestroySampler(device.getDevice(), m_sampler, nullptr);
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

// void ChunkDataSampler::_createSampler(const Device& device) {
//     VkSamplerCreateInfo samplerInfo{};
//     samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
//     samplerInfo.magFilter = VK_FILTER_NEAREST;
//     samplerInfo.minFilter = VK_FILTER_NEAREST;
//     samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
//     samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
//     samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
//     samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
//     samplerInfo.anisotropyEnable = VK_FALSE;
//     samplerInfo.maxAnisotropy = 1.0;
//     samplerInfo.compareEnable = VK_FALSE;
//     samplerInfo.compareOp = VK_COMPARE_OP_NEVER;
//     samplerInfo.mipLodBias = 0.0f;
//     samplerInfo.minLod = 0.0f;
//     samplerInfo.maxLod = 1.0f;
//     samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;

//     if (vkCreateSampler(device.getDevice(), &samplerInfo, nullptr, &m_sampler) != VK_SUCCESS) {
//         throw std::runtime_error("failed to create texture sampler!");
//     }
// }

} // namespace vox::gfx