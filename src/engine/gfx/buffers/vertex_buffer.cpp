/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vertex_buffer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 14:58:48 by etran             #+#    #+#             */
/*   Updated: 2024/05/29 12:37:36 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vertex_buffer.h"
#include "icommand_buffer.h"

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

/**
 * @brief Creates a vertex buffer.
 */
void VertexBuffer::init(
    const Device& device,
    const ICommandBuffer* cmdBuffer,
    const std::vector<VertexInstance>& instances
) {
    m_instancesCount = instances.size();

    BufferMetadata metadata{};
    metadata.m_size = sizeof(VertexInstance) * m_instancesCount;
    metadata.m_usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    metadata.m_properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    m_buffer.init(device, std::move(metadata));

    // Vertex buffer is static. Create staging buffer and copy data to it for
    // better performance.
    Buffer stagingBuffer = m_buffer.createStagingBuffer(device);
    stagingBuffer.map(device);
    stagingBuffer.copyFrom(instances.data());
    stagingBuffer.unmap(device);

    cmdBuffer->reset();
    cmdBuffer->startRecording();
    m_buffer.copyBuffer(cmdBuffer, stagingBuffer);
    cmdBuffer->stopRecording();
    cmdBuffer->awaitEndOfRecording(device);
    stagingBuffer.destroy(device);
}

void VertexBuffer::destroy(const Device& device) {
    m_buffer.destroy(device);
}

/* ========================================================================== */

const Buffer& VertexBuffer::getBuffer() const {
    return m_buffer;
}

u32 VertexBuffer::getInstancesCount() const noexcept {
    return m_instancesCount;
}

} // namespace vox::gfx