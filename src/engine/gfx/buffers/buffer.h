/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 17:14:27 by etran             #+#    #+#             */
/*   Updated: 2024/05/31 02:29:42 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vulkan/vulkan.h>

#include "types.h"

namespace vox::gfx {

class Device;
class ICommandBuffer;

/* ========================================================================== */
/*                               HELPER OBJECTS                               */
/* ========================================================================== */

struct BufferMetadata final {
    u32                     m_format;
    VkDeviceSize            m_size;
    VkBufferUsageFlags      m_usage;
    VkMemoryPropertyFlags   m_properties;
    VkSharingMode           m_sharingMode = VK_SHARING_MODE_EXCLUSIVE;
};

/**
 * @brief Wrapper class for VkBuffer and VkDeviceMemory.
*/
class Buffer final {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    Buffer() = default;
    Buffer(Buffer&& other) = default;
    Buffer(const Buffer& other) = default;
    Buffer& operator=(Buffer&& other) = default;
    Buffer& operator=(const Buffer& other) = default;
    ~Buffer() = default;

    /* ====================================================================== */

    void init(const Device& device, BufferMetadata&& metadata);
    void destroy(const Device& device);

    /* ====================================================================== */

    void map(const Device& device);
    void unmap(const Device& device);

    void copyTo(void* dst);
    void copyTo(void* dst, const u32 size);
    void copyFrom(const void* src);
    void copyFrom(const void* src, const u32 size, const u32 offset);

    void copyBuffer(
        const ICommandBuffer* cmdBuffer,
        const Buffer& src,
        const u32 srcOffset = 0,
        const u32 dstOffset = 0);

    Buffer  createStagingBuffer(const Device& device) const;

    /* ====================================================================== */

    VkBuffer                getBuffer() const noexcept;
    const BufferMetadata&   getMetadata() const noexcept;

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    BufferMetadata  m_metadata;

    VkBuffer        m_buffer = VK_NULL_HANDLE;
    VkDeviceMemory  m_memory = VK_NULL_HANDLE;
    void*           m_data = nullptr;

}; // class Buffer

} // namespace vox::gfx