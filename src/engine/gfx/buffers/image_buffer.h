/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_buffer.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 18:36:32 by etran             #+#    #+#             */
/*   Updated: 2024/06/14 02:41:17 by etran            ###   ########.fr       */
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

struct LayoutData final {
    VkImageLayout           m_layout = VK_IMAGE_LAYOUT_UNDEFINED;
    VkAccessFlags           m_accessMask = VK_ACCESS_NONE;
    VkPipelineStageFlags    m_stageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
};

struct ImageMetaData final {
    VkFormat                m_format;
    VkImageUsageFlags       m_usage;
    VkImageCreateFlags      m_flags = 0;
    u32                     m_queueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    VkImageType             m_imageType = VK_IMAGE_TYPE_2D;
    VkImageViewType         m_viewType = VK_IMAGE_VIEW_TYPE_2D;
    VkImageTiling           m_tiling = VK_IMAGE_TILING_OPTIMAL;
    VkSampleCountFlagBits   m_sampleCount = VK_SAMPLE_COUNT_1_BIT;
    VkMemoryPropertyFlags   m_properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    VkImageAspectFlags      m_aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
    LayoutData              m_layoutData{};

    u32 m_width;
    u32 m_height;
    u32 m_depth = 1;
    u32 m_mipCount = 1;
    u32 m_layerCount = 1;

    u32 getPixelSize() const;
    u32 getLayerSize() const noexcept;
    void computeMipCount();
};

/**
 * @brief Wrapper class for VkImage, VkDeviceMemory and VkImageView
 */
class ImageBuffer final {
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

    void initImage(const Device& device, ImageMetaData&& metadata);
    void destroy(const Device& device) const;
    void initView(const Device& device);

    /* ====================================================================== */

    void setLayout(
        const ICommandBuffer* cmdBuffer,
        const LayoutData& newData,
        const LayoutData* oldData = nullptr,
        const VkImageSubresourceRange* subresourceRange = nullptr,
        const u32 newQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED);
    void copyFrom(const ICommandBuffer* cmdBuffer, const Buffer& srcBuffer);
    void generateMipmap(const ICommandBuffer* cmdBuffer);

    Buffer createStagingBuffer(const Device& device) const;

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