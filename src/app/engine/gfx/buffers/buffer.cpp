/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 17:40:32 by etran             #+#    #+#             */
/*   Updated: 2024/03/01 00:08:23 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "buffer.h"
#include "device.h"
#include "icommand_buffer.h"

#include <cstring>
#include <stdexcept>

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
}

void Buffer::destroy(const Device& device) {
    vkDestroyBuffer(device.getDevice(), m_buffer, nullptr);
    vkFreeMemory(device.getDevice(), m_memory, nullptr);
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
void Buffer::copyFrom(const void* src, const u32 size) {
    memcpy(m_data, src, size);
}

void Buffer::copyFrom(const void* src) {
    memcpy(m_data, src, m_metadata.m_size);
}

/**
 * @brief Copies data from src buffer to dst buffer.
 */
void Buffer::copyBuffer(
    ICommandBuffer* cmdBuffer,
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
}

/* ========================================================================== */

VkBuffer Buffer::getBuffer() const noexcept {
    return m_buffer;
}

const BufferMetadata& Buffer::getMetadata() const noexcept {
    return m_metadata;
}

} // namespace vox::gfx