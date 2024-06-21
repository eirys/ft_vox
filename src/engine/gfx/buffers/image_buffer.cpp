/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_buffer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 18:36:30 by etran             #+#    #+#             */
/*   Updated: 2024/06/19 12:10:00 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "image_buffer.h"
#include "device.h"
#include "icommand_buffer.h"
#include "buffer.h"

#include <stdexcept>
#include <cmath>
#include "debug.h"

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

ImageBuffer::ImageBuffer(const bool isFramebuffer) {
    m_isFrameBuffer = isFramebuffer;
}

/* ========================================================================== */

void ImageBuffer::initImage(const Device& device, ImageMetaData&& metadata) {
    m_metadata = std::move(metadata);

    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = m_metadata.m_imageType;
    imageInfo.extent = { m_metadata.m_width, m_metadata.m_height, m_metadata.m_depth };
    imageInfo.mipLevels = m_metadata.m_mipCount;
    imageInfo.arrayLayers = m_metadata.m_layerCount;
    imageInfo.format = m_metadata.m_format;
    imageInfo.tiling = m_metadata.m_tiling;
    imageInfo.initialLayout = m_metadata.m_layoutData.m_layout;
    imageInfo.usage = m_metadata.m_usage;
    imageInfo.samples = m_metadata.m_sampleCount;
    imageInfo.flags = m_metadata.m_flags;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateImage(device.getDevice(), &imageInfo, nullptr, &m_image) != VK_SUCCESS) {
        throw std::runtime_error("failed to create image");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device.getDevice(), m_image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = device.findMemoryType(memRequirements.memoryTypeBits, m_metadata.m_properties);

    if (vkAllocateMemory(device.getDevice(), &allocInfo, nullptr, &m_memory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate image memory");
    } else if (vkBindImageMemory(device.getDevice(), m_image, m_memory, 0) != VK_SUCCESS) {
        throw std::runtime_error("failed to bind image memory");
    }

    if (m_isFrameBuffer) {
        if (m_metadata.m_usage & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT)
            m_metadata.m_layoutData.m_layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
        else if (m_metadata.m_usage & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)
            m_metadata.m_layoutData.m_layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    }

    LDEBUG("Image buffer initialized: " << "image::" << m_image
            << " | memory::" << m_memory);
}

void ImageBuffer::initView(const Device& device) {
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = m_image;
    viewInfo.viewType = m_metadata.m_viewType;
    viewInfo.format = m_metadata.m_format;
    viewInfo.subresourceRange.aspectMask = m_metadata.m_aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = m_metadata.m_mipCount;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = m_metadata.m_layerCount;

    if (vkCreateImageView(device.getDevice(), &viewInfo, nullptr, &m_view) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture image view");
    }

    LDEBUG("Image view initialized: view::" << m_view );
}

void ImageBuffer::destroy(const Device& device) const {
    vkDestroyImage(device.getDevice(), m_image, nullptr);
    vkFreeMemory(device.getDevice(), m_memory, nullptr);
    vkDestroyImageView(device.getDevice(), m_view, nullptr);
}

/* ========================================================================== */

/**
 * @brief Sets the layout of the image. Updates the metadata.
 */
void ImageBuffer::setLayout(
    const ICommandBuffer* cmdBuffer,
    const LayoutData& newData,
    const LayoutData* otherData,
    const VkImageSubresourceRange* subresourceRange,
    const u32 newQueueFamilyIndex
) {
    VkImageSubresourceRange range{};
    if (subresourceRange == nullptr) {
        range.aspectMask = m_metadata.m_aspectFlags;
        range.baseMipLevel = 0;
        range.baseArrayLayer = 0;
        range.levelCount = m_metadata.m_mipCount;
        range.layerCount = m_metadata.m_layerCount;
    } else {
        range = *subresourceRange;
    }

    LayoutData oldData{};
    if (otherData == nullptr) {
        oldData = m_metadata.m_layoutData;
    } else {
        oldData = *otherData;
    }

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.subresourceRange = range;
    barrier.image = m_image;

    barrier.oldLayout = oldData.m_layout;
    barrier.srcAccessMask = oldData.m_accessMask;
    barrier.srcQueueFamilyIndex = m_metadata.m_queueFamilyIndex;

    barrier.newLayout = newData.m_layout;
    barrier.dstAccessMask = newData.m_accessMask;
    barrier.dstQueueFamilyIndex = newQueueFamilyIndex;

    vkCmdPipelineBarrier(
        cmdBuffer->getBuffer(),
        oldData.m_stageMask, newData.m_stageMask,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier);

    if (subresourceRange == nullptr)
        m_metadata.m_layoutData = newData;
    m_metadata.m_queueFamilyIndex = newQueueFamilyIndex;
}

/**
 * @brief Copies data from a buffer to the image.
 * @note Assumes all layers have the same size.
 * @note Assumes there are no mipmaps.
 */
void ImageBuffer::copyFrom(const ICommandBuffer* cmdBuffer, const Buffer& srcBuffer) {
    constexpr LayoutData transferLayout {
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_ACCESS_TRANSFER_WRITE_BIT,
        VK_PIPELINE_STAGE_TRANSFER_BIT };
    setLayout(cmdBuffer, transferLayout);

    std::vector<VkBufferImageCopy> regions;
    regions.reserve(m_metadata.m_layerCount);

    for (u32 layer = 0; layer < m_metadata.m_layerCount; ++layer) {
        VkBufferImageCopy region{};
        region.bufferOffset = m_metadata.getLayerSize() * m_metadata.getPixelSize() * layer;
        region.imageExtent = { m_metadata.m_width, m_metadata.m_height, m_metadata.m_depth };
        region.imageSubresource.aspectMask = m_metadata.m_aspectFlags;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.layerCount = 1;
        region.imageSubresource.baseArrayLayer = layer;
        regions.emplace_back(region);
    }

    vkCmdCopyBufferToImage(
        cmdBuffer->getBuffer(),
        srcBuffer.getBuffer(),
        m_image,
        m_metadata.m_layoutData.m_layout,
        m_metadata.m_layerCount,
        regions.data());

    LDEBUG("Copied buffer to image::" << m_image);
}

void ImageBuffer::generateMipmap(const ICommandBuffer* cmdBuffer) {
    VkImageSubresourceRange subresourceRange{};
    subresourceRange.aspectMask = m_metadata.m_aspectFlags;

    constexpr LayoutData dstLayoutData {
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_ACCESS_TRANSFER_WRITE_BIT,
        VK_PIPELINE_STAGE_TRANSFER_BIT };

    for (u32 layer = 0; layer < m_metadata.m_layerCount; ++layer) {
        for (u32 level = 1; level < m_metadata.m_mipCount; ++level) {
            // Define blitting regions
            VkImageBlit blit{};

            // Src
            blit.srcSubresource.aspectMask = m_metadata.m_aspectFlags;
            blit.srcSubresource.baseArrayLayer = layer;
            blit.srcSubresource.layerCount = 1;
            blit.srcSubresource.mipLevel = level - 1;
            blit.srcOffsets[0] = {0, 0, 0};
            blit.srcOffsets[1].x = m_metadata.m_width >> (level - 1);
            blit.srcOffsets[1].y = m_metadata.m_height >> (level - 1);
            blit.srcOffsets[1].z = m_metadata.m_depth;

            // Dst
            blit.dstSubresource.aspectMask = m_metadata.m_aspectFlags;
            blit.dstSubresource.baseArrayLayer = layer;
            blit.dstSubresource.layerCount = 1;
            blit.dstSubresource.mipLevel = level;
            blit.dstOffsets[0] = {0, 0, 0};
            blit.dstOffsets[1].x = m_metadata.m_width >> level;
            blit.dstOffsets[1].y = m_metadata.m_height >> level;
            blit.dstOffsets[1].z = m_metadata.m_depth;

            // Setup layout for blitting
            subresourceRange.baseMipLevel = level - 1;
            subresourceRange.baseArrayLayer = layer;
            subresourceRange.levelCount = 1;
            subresourceRange.layerCount = 1;

            constexpr LayoutData srcLayoutData {
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                VK_ACCESS_TRANSFER_WRITE_BIT,
                VK_PIPELINE_STAGE_TRANSFER_BIT };
            constexpr LayoutData midLayoutData {
                VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                VK_ACCESS_TRANSFER_READ_BIT,
                VK_PIPELINE_STAGE_TRANSFER_BIT };

            setLayout(
                cmdBuffer,
                midLayoutData,
                &srcLayoutData,
                &subresourceRange);

            // Perform blit
            vkCmdBlitImage(
                cmdBuffer->getBuffer(),
                m_image, midLayoutData.m_layout,
                m_image, dstLayoutData.m_layout,
                1, &blit,
                VK_FILTER_LINEAR);

            setLayout(
                cmdBuffer,
                dstLayoutData,
                &midLayoutData,
                &subresourceRange);
        }
    }

    constexpr LayoutData shaderLayoutData {
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        VK_ACCESS_SHADER_READ_BIT,
        VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT };
    setLayout(cmdBuffer, shaderLayoutData, &dstLayoutData);

    LDEBUG("Generated mipmaps for image::" << m_image);
}

/**
 * @brief Creates a staging buffer with proper format.
 * @note The buffer needs to be mapped prior to copying data, and unmapped after before destruction.
 */
Buffer ImageBuffer::createStagingBuffer(const Device& device) const {
    BufferMetadata  bufferData{};
    bufferData.m_format = m_metadata.getPixelSize();
    bufferData.m_size = m_metadata.m_width *
                        m_metadata.m_height *
                        m_metadata.m_layerCount;
    bufferData.m_usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    bufferData.m_properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    Buffer stagingBuffer;
    stagingBuffer.init(device, std::move(bufferData));

    return stagingBuffer;
}

/* ========================================================================== */

VkImage ImageBuffer::getImage() const noexcept {
    return m_image;
}

VkImageView ImageBuffer::getView() const noexcept {
    return m_view;
}

const ImageMetaData& ImageBuffer::getMetaData() const noexcept {
    return m_metadata;
}

/* ========================================================================== */
/*                                    OTHER                                   */
/* ========================================================================== */

u32  ImageMetaData::getPixelSize() const {
    switch (m_format) {
        // u8
        case VK_FORMAT_R8_UINT:
        case VK_FORMAT_R8_UNORM:
            return sizeof(u8);

        case VK_FORMAT_R8G8_UINT:
            return sizeof(u8) * 2;

        case VK_FORMAT_R8G8B8_UINT:
        case VK_FORMAT_B8G8R8_UINT:
            return sizeof(u8) * 3;

        case VK_FORMAT_R8G8B8A8_SRGB:
        case VK_FORMAT_R8G8B8A8_UINT:
        case VK_FORMAT_B8G8R8A8_UINT:
            return sizeof(u8) * 4;

        // u16
        case VK_FORMAT_R16_UINT:
            return sizeof(u16);

        case VK_FORMAT_R16G16_UINT:
            return sizeof(u16) * 2;

        case VK_FORMAT_R16G16B16_UINT:
            return sizeof(u16) * 3;

        case VK_FORMAT_R16G16B16A16_SFLOAT:
        case VK_FORMAT_R16G16B16A16_UINT:
            return sizeof(u16) * 4;

        // u32
        case VK_FORMAT_R32_SFLOAT:
        case VK_FORMAT_R32_UINT:
            return sizeof(u32);

        case VK_FORMAT_R32G32_UINT:
        case VK_FORMAT_R32G32_SFLOAT:
            return sizeof(u32) * 2;

        case VK_FORMAT_R32G32B32_UINT:
            return sizeof(u32) * 3;

        case VK_FORMAT_R32G32B32A32_SFLOAT:
        case VK_FORMAT_R32G32B32A32_UINT:
            return sizeof(u32) * 4;

        // u64
        case VK_FORMAT_R64_UINT:
            return sizeof(u64);

        case VK_FORMAT_R64G64_UINT:
            return sizeof(u64) * 2;


        default:
            throw std::runtime_error("pixel format not supported");
            return 0;
    }
}

u32  ImageMetaData::getLayerSize() const noexcept {
    return m_width * m_height;
}

void ImageMetaData::computeMipCount() {
	m_mipCount = 1 + (u32)std::floor(std::log2(m_width));
}

} // namespace vox::gfx