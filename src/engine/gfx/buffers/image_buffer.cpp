/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_buffer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 18:36:30 by etran             #+#    #+#             */
/*   Updated: 2024/03/11 14:28:47 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "image_buffer.h"
#include "device.h"
#include "icommand_buffer.h"
#include "buffer.h"

#include <stdexcept>
#include "debug.h"

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void ImageBuffer::init(const Device& device, ImageMetaData&& metadata) {
    m_metadata = std::move(metadata);

    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = m_metadata.m_imageType;
    imageInfo.extent = { m_metadata.m_width, m_metadata.m_height, m_metadata.m_depth };
    imageInfo.mipLevels = m_metadata.m_mipCount;
    imageInfo.arrayLayers = m_metadata.m_layerCount;
    imageInfo.format = m_metadata.m_format;
    imageInfo.tiling = m_metadata.m_tiling;
    imageInfo.initialLayout = m_metadata.m_layout;
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
    LDEBUG("Initialized " << __NL
    << "  - image buffer " << m_image << __NL
    << "  - View: " << m_view << __NL
    << "  - Memory: " << m_memory << __NL);
}

void ImageBuffer::destroy(const Device& device) {
    vkDestroyImage(device.getDevice(), m_image, nullptr);
    vkFreeMemory(device.getDevice(), m_memory, nullptr);
    vkDestroyImageView(device.getDevice(), m_view, nullptr);
}

/* ========================================================================== */

/**
 * @brief Sets the layout of the image. Updates the metadata.
 */
void ImageBuffer::setLayout(
    ICommandBuffer* cmdBuffer,
    const VkImageSubresourceRange& subresourceRange,
    const VkImageLayout newLayout,
    const VkAccessFlags newAccessMask,
    const VkPipelineStageFlags newStageMask,
    const u32 newQueueFamilyIndex
) {
    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.subresourceRange = subresourceRange;
    barrier.image = m_image;

    barrier.oldLayout = m_metadata.m_layout;
    barrier.srcAccessMask = m_metadata.m_accessMask;
    barrier.srcQueueFamilyIndex = m_metadata.m_queueFamilyIndex;

    barrier.newLayout = newLayout;
    barrier.dstAccessMask = newAccessMask;
    barrier.dstQueueFamilyIndex = newQueueFamilyIndex;

    vkCmdPipelineBarrier(
        cmdBuffer->getBuffer(),
        m_metadata.m_stageMask, newStageMask,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier);

    m_metadata.m_layout = newLayout;
    m_metadata.m_accessMask = newAccessMask;
    m_metadata.m_queueFamilyIndex = newQueueFamilyIndex;
    m_metadata.m_stageMask = newStageMask;
}

/**
 * @brief Copies data from a buffer to the image.
 * @note Assumes all layers have the same size.
 * @note Assumes there are no mipmaps.
 */
void ImageBuffer::copyFrom(ICommandBuffer* cmdBuffer, const Buffer& srcBuffer) {
    std::vector<VkBufferImageCopy> regions;
    regions.reserve(m_metadata.m_mipCount);

    for (u32 layer = 0; layer < m_metadata.m_layerCount; ++layer) {
        VkBufferImageCopy region{};
        region.bufferOffset = m_metadata.m_layerSize * layer;
        region.imageExtent = { m_metadata.m_width, m_metadata.m_height, m_metadata.m_depth };
        region.imageSubresource.aspectMask = m_metadata.m_aspectFlags;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.layerCount = 1;
        region.imageSubresource.baseArrayLayer = layer;
        regions.emplace_back(region);
    }

    constexpr VkImageLayout newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    vkCmdCopyBufferToImage(
        cmdBuffer->getBuffer(),
        srcBuffer.getBuffer(),
        m_image,
        newLayout,
        (u32)regions.size(),
        regions.data());

    m_metadata.m_layout = newLayout;
}

void ImageBuffer::generateMipmap(ICommandBuffer* cmdBuffer) {
    VkImageSubresourceRange subresourceRange{};
    subresourceRange.aspectMask = m_metadata.m_aspectFlags;

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

            setLayout(
                cmdBuffer,
                subresourceRange,
                VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                VK_ACCESS_TRANSFER_READ_BIT,
                VK_PIPELINE_STAGE_TRANSFER_BIT);

            // Perform blit
            vkCmdBlitImage(
                cmdBuffer->getBuffer(),
                m_image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                m_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                1, &blit,
                VK_FILTER_LINEAR);

            setLayout(
                cmdBuffer,
                subresourceRange,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                VK_ACCESS_TRANSFER_WRITE_BIT,
                VK_PIPELINE_STAGE_TRANSFER_BIT);
        }
    }

    // // Revert layout to shader read layout
    // subresourceRange.baseMipLevel = 0;
    // subresourceRange.baseArrayLayer = 0;
    // subresourceRange.levelCount = m_metadata.m_mipCount;
    // subresourceRange.layerCount = m_metadata.m_layerCount;

    // setLayout(
    //     cmdBuffer,
    //     subresourceRange,
    //     VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
    //     VK_ACCESS_TRANSFER_READ_BIT,
    //     VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);
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

constexpr u32  ImageMetaData::getPixelSize() const noexcept {
    switch (m_format) {
        // uint
        case VK_FORMAT_R8_UINT:
            return sizeof(u8);

        case VK_FORMAT_R16_UINT:
        case VK_FORMAT_R8G8_UINT:
            return sizeof(u8);

        case VK_FORMAT_R8G8B8_UINT:
        case VK_FORMAT_B8G8R8_UINT:
            return sizeof(u8) * 3;

        case VK_FORMAT_R32_UINT:
        case VK_FORMAT_R16G16_UINT:
        case VK_FORMAT_R8G8B8A8_UINT:
        case VK_FORMAT_B8G8R8A8_UINT:
            return sizeof(u8) * 4;


        case VK_FORMAT_R16G16B16_UINT:
            return sizeof(u16) * 3;

        case VK_FORMAT_R64_UINT:
        case VK_FORMAT_R32G32_UINT:
        case VK_FORMAT_R16G16B16A16_UINT:
            return sizeof(u64);

        case VK_FORMAT_R32G32B32_UINT:
            return sizeof(u32) * 3;

        case VK_FORMAT_R64G64_UINT:
        case VK_FORMAT_R32G32B32A32_UINT:
            return sizeof(u64) * 2;

        // sfloat
        case VK_FORMAT_R8G8B8A8_SRGB:
            return sizeof(u32);

        default:
            return 0;
    }
}

} // namespace vox::gfx