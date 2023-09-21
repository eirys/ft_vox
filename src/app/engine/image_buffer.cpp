/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_buffer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 09:57:49 by etran             #+#    #+#             */
/*   Updated: 2023/08/10 22:09:32 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "image_buffer.h"
#include "device.h"
#include "utils.h"

#include <stdexcept> // std::runtime_error
#include <functional> // std::function

namespace scop::graphics {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

/**
 * @brief Create vkImage and allocate memory.
*/
void ImageBuffer::initImage(
	Device& device,
	uint32_t width,
	uint32_t height,
	VkFormat pixel_format,
	VkImageUsageFlags usage,
	VkSampleCountFlagBits sample_count,
	uint32_t mip_count,
	uint32_t layer_count,
	VkImageCreateFlags flags,
	VkMemoryPropertyFlags properties,
	VkImageTiling tiling
) {
	// Create vkimage
	VkImageCreateInfo	image_info{};
	image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	image_info.imageType = VK_IMAGE_TYPE_2D;
	image_info.extent = { width, height, 1 };
	image_info.mipLevels = mip_count;
	image_info.arrayLayers = layer_count;
	image_info.format = pixel_format;
	image_info.tiling = tiling;
	image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	image_info.usage = usage;
	image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	image_info.samples = sample_count;
	image_info.flags = flags;

	if (vkCreateImage(device.getLogicalDevice(), &image_info, nullptr, &_image) != VK_SUCCESS) {
		throw std::runtime_error("failed to create image");
	}

	// Look for memory requirements for allcoation
	VkMemoryRequirements	mem_requirements;
	vkGetImageMemoryRequirements(
		device.getLogicalDevice(),
		_image,
		&mem_requirements
	);

	// Verify if memory type is compatible with properties
	VkMemoryAllocateInfo	alloc_info{};
	alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	alloc_info.allocationSize = mem_requirements.size;
	alloc_info.memoryTypeIndex = device.findMemoryType(
		mem_requirements.memoryTypeBits,
		properties
	);

	if (vkAllocateMemory(device.getLogicalDevice(), &alloc_info, nullptr, &_memory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate image memory");
	} else if (vkBindImageMemory(device.getLogicalDevice(), _image, _memory, 0) != VK_SUCCESS) {
		throw std::runtime_error("failed to bind image memory");
	}
}

/**
 * @brief Create associated image view.
 *
 * @param device Engine device.
 * @param image_format Format of the image.
 * @param aspect_flags Aspect flags for the image.
 * @param view_type View type (2D, Cube map).
 * @param mip_count Number of mip levels.
 * @param layer_count Number of layers * textures.
*/
void	ImageBuffer::initView(
	Device& device,
	VkFormat image_format,
	VkImageAspectFlags aspect_flags,
	VkImageViewType view_type,
	uint32_t mip_count,
	uint32_t layer_count
) {
	VkImageViewCreateInfo	view_info{};
	view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	view_info.image = _image;
	view_info.viewType = view_type;
	view_info.format = image_format;
	view_info.subresourceRange.aspectMask = aspect_flags;
	view_info.subresourceRange.baseMipLevel = 0;
	view_info.subresourceRange.levelCount = mip_count;
	view_info.subresourceRange.baseArrayLayer = 0;
	view_info.subresourceRange.layerCount = layer_count;

	if (vkCreateImageView(device.getLogicalDevice(), &view_info, nullptr, &_view) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture image view");
	}
}

void	ImageBuffer::destroy(Device& device) {
	vkDestroyImage(device.getLogicalDevice(), _image, nullptr);
	vkFreeMemory(device.getLogicalDevice(), _memory, nullptr);
	vkDestroyImageView(device.getLogicalDevice(), _view, nullptr);
}

/* ========================================================================== */

/**
 * @brief Send command to transtion image layout.
*/
void	ImageBuffer::setLayout(
	VkCommandBuffer command_buffer,
	VkImageLayout old_layout,
	VkImageLayout new_layout,
	VkAccessFlags src_access_mask,
	VkAccessFlags dst_access_mask,
	VkPipelineStageFlags src_stage_mask,
	VkPipelineStageFlags dst_stage_mask,
	const VkImageSubresourceRange& subresource_range
) {
	VkImageMemoryBarrier	barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.image = _image;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.oldLayout = old_layout;
	barrier.newLayout = new_layout;
	barrier.srcAccessMask = src_access_mask;
	barrier.dstAccessMask = dst_access_mask;
	barrier.subresourceRange = subresource_range;

	vkCmdPipelineBarrier(
		command_buffer,
		src_stage_mask,
		dst_stage_mask,
		0,
		0, nullptr,
		0, nullptr,
		1, &barrier
	);
}

/**
 * @brief Copy data from a staging buffer to image.
 *
 * @note Assuming that all images have the same size
 * @note Assuming there are no mipmaps.
 * @note Assuming all images have the same number of layers.
 *
 * @param command_buffer Command buffer to send the copy command.
 * @param src_buffer Staging buffer containing the data to copy.
 * @param image_width Width of the VkImage.
 * @param image_height Height of the VkImage.
 * @param image_count Number of image files. Default is 1.
 * @param layer_count Number of layers. 6 for cube maps. Default is 1.
 * @param layer_size Size of a layer in bytes. Default is 0.
 * @param pixel_size Size of a pixel in bytes. Default is sizeof(uint32_t).
*/
void	ImageBuffer::copyFrom(
	VkCommandBuffer command_buffer,
	VkBuffer src_buffer,
	uint32_t image_width,
	uint32_t image_height,
	uint32_t image_count,
	uint32_t layer_count,
	uint32_t layer_size,
	uint32_t pixel_size
) {
	// Create regions for transfer operation (define layout)
	VkDeviceSize	image_size = image_width * image_height * pixel_size;

	std::vector<VkBufferImageCopy>	regions;
	regions.reserve(image_count * layer_count);

	VkDeviceSize	layer_offset = 0;
	for (uint32_t layer = 0; layer < layer_count; layer++) {
		for (uint32_t image = 0; image < image_count; image++) {
			VkBufferImageCopy	region{};
			region.bufferOffset =
				image_size * layer_count * image +	// offset for image
				layer_offset;						// offset for layer
			region.imageExtent = { image_width, image_height, 1 };
			region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			region.imageSubresource.mipLevel = 0;
			region.imageSubresource.layerCount = 1;
			region.imageSubresource.baseArrayLayer =
				image * layer_count +
				layer;

			regions.emplace_back(region);
		}
		layer_offset += layer_size;
	}

	// Send copy command
	vkCmdCopyBufferToImage(
		command_buffer,
		src_buffer,
		_image,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		static_cast<uint32_t>(regions.size()),
		regions.data()
	);
}

/**
 * @brief Generate mipmaps for the image.
 *
 * @note Assumes that the image has the proper layout to begin with.
 *
 * @param command_buffer Command buffer to send the copy command.
 * @param device Device used to create the image.
 * @param image_width Width of the VkImage.
 * @param image_height Height of the VkImage.
 * @param image_format Format of the VkImage.
 * @param mip_count Number of mipmaps.
 * @param layer_count Number of layers. 6 for cube maps.
*/
void				ImageBuffer::generateMipmap(
	VkCommandBuffer command_buffer,
	Device& device,
	uint32_t image_width,
	uint32_t image_height,
	VkFormat image_format,
	uint32_t mip_count,
	uint32_t layer_count
) {
	// Check if support blitting
	VkFormatProperties	properties;
	vkGetPhysicalDeviceFormatProperties(
		device.getPhysicalDevice(),
		image_format,
		&properties
	);

	if (!(properties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
		throw std::runtime_error("texture image format doesn't support linear blitting");
	}

	VkImageSubresourceRange	range{};
	range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

	for (uint32_t layer = 0; layer < layer_count; ++layer) {
		for (uint32_t level = 1; level < mip_count; ++level) {
			// Define blitting regions
			VkImageBlit	blit{};

			// Source
			blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.srcSubresource.baseArrayLayer = layer;
			blit.srcSubresource.mipLevel = level - 1;
			blit.srcSubresource.layerCount = 1;
			blit.srcOffsets[0] = { 0, 0, 0 };
			blit.srcOffsets[1].x = image_width >> (level - 1);
			blit.srcOffsets[1].y = image_height >> (level - 1);
			blit.srcOffsets[1].z = 1;

			// Destination
			blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.dstSubresource.baseArrayLayer = layer;
			blit.dstSubresource.mipLevel = level;
			blit.dstSubresource.layerCount = 1;
			blit.dstOffsets[0] = { 0, 0, 0 };
			blit.dstOffsets[1].x = image_width >> level;
			blit.dstOffsets[1].y = image_height >> level;
			blit.dstOffsets[1].z = 1;

			// Set range for layout transition
			range.baseMipLevel = level - 1;
			range.baseArrayLayer = layer;
			range.levelCount = 1;
			range.layerCount = 1;

			// Transition previous layer as transfer source
			setLayout(
				command_buffer,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				VK_ACCESS_TRANSFER_WRITE_BIT,
				VK_ACCESS_TRANSFER_READ_BIT,
				VK_PIPELINE_STAGE_TRANSFER_BIT,
				VK_PIPELINE_STAGE_TRANSFER_BIT,
				range
			);

			// Perform blit
			vkCmdBlitImage(
				command_buffer,
				_image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1, &blit,
				VK_FILTER_LINEAR
			);

			// Transition back to destination layout
			setLayout(
				command_buffer,
				VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				VK_ACCESS_TRANSFER_READ_BIT,
				VK_ACCESS_TRANSFER_WRITE_BIT,
				VK_PIPELINE_STAGE_TRANSFER_BIT,
				VK_PIPELINE_STAGE_TRANSFER_BIT,
				range
			);
		}
	}

	// Revert layout of all level to shader read
	range.baseMipLevel = 0;
	range.baseArrayLayer = 0;
	range.layerCount = layer_count;
	range.levelCount = mip_count;

	setLayout(
		command_buffer,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
		VK_ACCESS_TRANSFER_WRITE_BIT,
		VK_ACCESS_SHADER_READ_BIT,
		VK_PIPELINE_STAGE_TRANSFER_BIT,
		VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
		range
	);
}

/* ========================================================================== */

VkImage	ImageBuffer::getImage() const noexcept {
	return _image;
}

VkImageView	ImageBuffer::getView() const noexcept {
	return _view;
}

} // namespace vox