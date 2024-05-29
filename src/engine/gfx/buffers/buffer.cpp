/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 17:40:32 by etran             #+#    #+#             */
/*   Updated: 2024/05/29 12:37:30 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "buffer.h"
#include "device.h"
#include "icommand_buffer.h"

#include <cstring>
#include <stdexcept>

#include "debug.h"

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

/**
 * @brief Creates buffer and bind allocated memory.
 */
void Buffer::init(const Device& device, BufferMetadata&& metadata) {
    m_metadata = std::move(metadata);

    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = m_metadata.m_size;
    bufferInfo.usage = m_metadata.m_usage;
    bufferInfo.sharingMode = m_metadata.m_sharingMode;

    if (vkCreateBuffer(device.getDevice(), &bufferInfo, nullptr, &m_buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device.getDevice(), m_buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = device.findMemoryType(memRequirements.memoryTypeBits, m_metadata.m_properties);

    if (vkAllocateMemory(device.getDevice(), &allocInfo, nullptr, &m_memory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory");
    } else if (vkBindBufferMemory(device.getDevice(), m_buffer, m_memory, 0) != VK_SUCCESS) {
        throw std::runtime_error("failed to bind buffer memory");
    }

    LDEBUG("Buffer created: buffer::"<<m_buffer<<" | memory::"<<m_memory);
}

void Buffer::destroy(const Device& device) {
    vkDestroyBuffer(device.getDevice(), m_buffer, nullptr);
    vkFreeMemory(device.getDevice(), m_memory, nullptr);
    LDEBUG("Buffer destroyed:" << m_buffer);
}

/* ========================================================================== */

/**
 * @brief Maps buffer memory to CPU accessible memory.
 */
void Buffer::map(const Device& device) {
    if (vkMapMemory(device.getDevice(), m_memory, 0, m_metadata.m_size, 0, &m_data) != VK_SUCCESS) {
        throw std::runtime_error("failed to map buffer memory");
    }
}

/**
 * @brief Unmaps buffer memory.
 */
void Buffer::unmap(const Device& device) {
    vkUnmapMemory(device.getDevice(), m_memory);
}

/**
 * @brief Copies data to CPU dst buffer.
 * @attention Must be mapped first.
 */
void Buffer::copyTo(void* dst, const u32 size) {
    memcpy(dst, m_data, size);
}

void Buffer::copyTo(void* dst) {
    memcpy(dst, m_data, m_metadata.m_size);
}

/**
 * @brief Copies data from CPU src buffer.
 * @attention Must be mapped first.
 */
void Buffer::copyFrom(const void* src, const u32 size, const u32 offset) {
    u8* data = (u8*)m_data;
    memcpy(data + offset, src, size);
}

void Buffer::copyFrom(const void* src) {
    memcpy(m_data, src, m_metadata.m_size);

    LDEBUG("Buffer " << m_buffer << " copied from CPU.");
}

/**
 * @brief Copies data from src buffer to dst buffer.
 */
void Buffer::copyBuffer(
    const ICommandBuffer* cmdBuffer,
    const Buffer& src,
    const u32 srcOffset,
    const u32 dstOffset
) {
    VkBufferCopy copyRegion = {};
    copyRegion.srcOffset = srcOffset;
    copyRegion.dstOffset = dstOffset;
    copyRegion.size = m_metadata.m_size;

    vkCmdCopyBuffer(
        cmdBuffer->getBuffer(),
        src.getBuffer(),
        m_buffer,
        1, &copyRegion);

    LDEBUG("Buffer " << m_buffer << " copied from " << src.getBuffer() << ".");
}

Buffer Buffer::createStagingBuffer(const Device& device) const {
    BufferMetadata stagingMetadata{};
    stagingMetadata.m_size = m_metadata.m_size;
    stagingMetadata.m_usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    stagingMetadata.m_properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    Buffer stagingBuffer{};
    stagingBuffer.init(device, std::move(stagingMetadata));
    return stagingBuffer;
}

/* ========================================================================== */

VkBuffer Buffer::getBuffer() const noexcept {
    return m_buffer;
}

const BufferMetadata& Buffer::getMetadata() const noexcept {
    return m_metadata;
}

} // namespace vox::gfx