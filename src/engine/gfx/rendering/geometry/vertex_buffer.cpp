/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vertex_buffer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 14:58:48 by etran             #+#    #+#             */
/*   Updated: 2024/08/15 17:35:23 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vertex_buffer.h"
#include "icommand_buffer.h"
#include "game_state.h"
#include "bounding_box.h"
#include "bounding_frustum.h"

#include "debug.h"

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

Buffer  VertexBuffer::ms_buffer;
u32     VertexBuffer::ms_instancesCount = 0;

void VertexBuffer::init(const Device& device, const ICommandBuffer* cmdBuffer) {
    const auto& totalInstances = game::GameState::getWorld().getInstances();

    ms_instancesCount = totalInstances.size();

    BufferMetadata metadata{};
    metadata.m_format = sizeof(VertexInstance);
    metadata.m_size = ms_instancesCount;
    metadata.m_usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    metadata.m_properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    ms_buffer.init(device, std::move(metadata));

    Buffer  stagingBuffer = ms_buffer.createStagingBuffer(device);
    stagingBuffer.map(device);
    stagingBuffer.copyFrom(totalInstances.data(), sizeof(VertexInstance) * ms_instancesCount, 0);
    stagingBuffer.unmap(device);

    cmdBuffer->reset();
    cmdBuffer->startRecording();
    ms_buffer.copyBuffer(cmdBuffer, stagingBuffer);
    cmdBuffer->stopRecording();
    cmdBuffer->awaitEndOfRecording(device);

    stagingBuffer.destroy(device);
    LINFO("Vertex buffer initialized.");
}

void VertexBuffer::destroy(const Device& device) {
    ms_buffer.destroy(device);
}

void VertexBuffer::bind(const ICommandBuffer* cmdBuffer) {
    const VkDeviceSize  offset = 0;
    const VkBuffer      buffer = ms_buffer.getBuffer();

    vkCmdBindVertexBuffers(cmdBuffer->getBuffer(), 0, 1, &buffer, &offset);
}

/* ========================================================================== */

const Buffer& VertexBuffer::getBuffer() noexcept {
    return ms_buffer;
}

u32 VertexBuffer::getInstancesCount() noexcept {
    return ms_instancesCount;
}

} // namespace vox::gfx