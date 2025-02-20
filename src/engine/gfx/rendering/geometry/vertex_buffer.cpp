/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vertex_buffer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 14:58:48 by etran             #+#    #+#             */
/*   Updated: 2024/09/17 15:30:31 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vertex_buffer.h"
#include "icommand_buffer.h"
#include "game_state.h"

#include "debug.h"

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

std::vector<Buffer>     VertexBuffer::ms_buffers;
u32                     VertexBuffer::ms_currentBuffer = 0;

void VertexBuffer::init(const Device& device, const ICommandBuffer* cmdBuffer) {
    ms_buffers.resize(BUFFER_COUNT);
    update(device, cmdBuffer);
}

void VertexBuffer::destroy(const Device& device) {
    for (Buffer& buffer : ms_buffers) buffer.destroy(device);
}

void VertexBuffer::update(const Device& device, const ICommandBuffer* cmdBuffer) {
    const auto& vertexInstances = game::GameState::getWorld().getInstances();

    // Create vertex buffer
    BufferMetadata metadata{};
    metadata.m_format = sizeof(VertexInstance);
    metadata.m_size = vertexInstances.size();
    metadata.m_usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    metadata.m_properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    ms_buffers[ms_currentBuffer].init(device, std::move(metadata));
    LDEBUG("Total instances :" << vertexInstances.size());

    // Copy data to staging buffer
    Buffer  stagingBuffer = ms_buffers[ms_currentBuffer].createStagingBuffer(device);
    stagingBuffer.map(device);
    stagingBuffer.copyFrom(vertexInstances.data());
    stagingBuffer.unmap(device);

    cmdBuffer->reset();
    cmdBuffer->startRecording();
    ms_buffers[ms_currentBuffer].copyBuffer(cmdBuffer, stagingBuffer);
    cmdBuffer->stopRecording();
    cmdBuffer->awaitEndOfRecording(device);

    stagingBuffer.destroy(device);

    LINFO("Vertex buffer " << ms_currentBuffer << " updated.");
}

void VertexBuffer::bind(const ICommandBuffer* cmdBuffer) {
    const VkDeviceSize  offset = 0;
    const VkBuffer      buffer = ms_buffers[ms_currentBuffer].getBuffer();

    vkCmdBindVertexBuffers(cmdBuffer->getBuffer(), 0, 1, &buffer, &offset);
}

void VertexBuffer::changeBuffer() noexcept {
    ms_currentBuffer = (ms_currentBuffer + 1) % BUFFER_COUNT;
}

} // namespace vox::gfx