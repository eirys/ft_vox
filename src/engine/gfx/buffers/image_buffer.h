/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_buffer.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 18:36:32 by etran             #+#    #+#             */
/*   Updated: 2024/03/01 00:08:13 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vulkan/vulkan.h>
#include "types.h"

namespace vox::gfx {

class Device;
class ICommandBuffer;
class Buffer;

/* ========================================================================== */
/*                               HELPER OBJECTS                               */
/* ========================================================================== */

struct ImageMetaData {
    VkFormat                m_format;
    u32                     m_queueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    VkImageType             m_imageType = VK_IMAGE_TYPE_2D;
    VkImageViewType         m_viewType = VK_IMAGE_VIEW_TYPE_2D;
    VkImageTiling           m_tiling = VK_IMAGE_TILING_OPTIMAL;
    VkImageLayout           m_layout = VK_IMAGE_LAYOUT_UNDEFINED;
    VkSampleCountFlagBits   m_sampleCount = VK_SAMPLE_COUNT_1_BIT;

    VkImageUsageFlags       m_usage;
    VkImageCreateFlags      m_flags = 0;
    VkAccessFlags           m_accessMask = VK_ACCESS_NONE;
    VkPipelineStageFlags    m_stageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    VkMemoryPropertyFlags   m_properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    VkImageAspectFlags      m_aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;

    u32 m_width;
    u32 m_height;
    u32 m_depth = 1;
    u32 m_mipCount = 1;
    u32 m_layerCount = 1;
    u32 m_layerSize = 0;

    constexpr
    u32 getPixelSize() const noexcept;
};

/**
 * @brief Wrapper class for VkImage, VkDeviceMemory and VkImageView
 */
class ImageBuffer {
public:
    /* ====================================================================== */
    /*                                 PUBLIC                                 */
    /* ====================================================================== */

    ImageBuffer() = default;
    ImageBuffer(ImageBuffer&& other) = default;
    ImageBuffer(const ImageBuffer& other) = default;
    ImageBuffer& operator=(ImageBuffer&& other) = default;
    ImageBuffer& operator=(const ImageBuffer& other) = default;
    ~ImageBuffer() = default;

    /* ====================================================================== */

    void init(const Device& device, ImageMetaData&& metadata);
    void destroy(const Device& device);

    /* ====================================================================== */

    void setLayout(
        ICommandBuffer* cmdBuffer,
        const VkImageSubresourceRange& subresourceRange,
        const VkImageLayout newLayout,
        const VkAccessFlags newAccessMask,
        const VkPipelineStageFlags newStageMask,
        const u32 newQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED);
    void copyFrom(ICommandBuffer* cmdBuffer, const Buffer& srcBuffer);
    void generateMipmap(ICommandBuffer* cmdBuffer);

    /* ====================================================================== */

    VkImage                 getImage() const noexcept;
    VkImageView             getView() const noexcept;
    const ImageMetaData&    getMetaData() const noexcept;

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    ImageMetaData   m_metadata;

    VkImage         m_image = VK_NULL_HANDLE;
    VkDeviceMemory  m_memory = VK_NULL_HANDLE;
    VkImageView     m_view = VK_NULL_HANDLE;
};

} // namespace vox::gfx