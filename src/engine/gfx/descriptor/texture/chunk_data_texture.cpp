/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_data_texture.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 23:05:38 by etran             #+#    #+#             */
/*   Updated: 2024/08/22 15:30:31 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chunk_data_texture.h"
#include "device.h"
#include "buffer.h"
#include "icommand_buffer.h"
#include "game_state.h"
#include "debug.h"

#include <stdexcept>

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void ChunkDataTexture::init(const Device& device) {
    const u32 renderArea = game::World::getSettings().rendering.getRenderedChunksCount();

    ImageMetaData textureData{};
    textureData.m_format = VK_FORMAT_R8_UINT;
    textureData.m_width = CHUNK_SIZE * 16;
    textureData.m_height = CHUNK_SIZE;
    textureData.m_layerCount = renderArea; // A layer = a chunk
    textureData.m_usage = VK_IMAGE_USAGE_SAMPLED_BIT |      // Sampled texture
                          VK_IMAGE_USAGE_TRANSFER_DST_BIT;  // Transfer destination
    textureData.m_viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
    m_imageBuffer.initImage(device, std::move(textureData));

    LDEBUG("Created Chunk data text");
}

void ChunkDataTexture::fill(const Device& device, const ICommandBuffer* cmdBuffer) {
    constexpr LayoutData finalLayout{
        .m_layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        .m_accessMask = VK_ACCESS_SHADER_READ_BIT,
        .m_stageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT };

    LDEBUG("Filling Chunk data text");
    Buffer stagingBuffer = m_imageBuffer.createStagingBuffer(device);
    stagingBuffer.map(device);
    stagingBuffer.copyFrom(game::GameState::getWorld().getBlockRaw().data());
    stagingBuffer.unmap(device);
    LDEBUG("Copied");

    cmdBuffer->reset();
    cmdBuffer->startRecording();
    m_imageBuffer.copyFrom(cmdBuffer, stagingBuffer);
    m_imageBuffer.setLayout(cmdBuffer, finalLayout);
    cmdBuffer->stopRecording();
    cmdBuffer->awaitEndOfRecording(device);
    stagingBuffer.destroy(device);

    m_imageBuffer.initView(device);
}

void ChunkDataTexture::destroy(const Device& device) {
    m_imageBuffer.destroy(device);
}

} // namespace vox::gfx