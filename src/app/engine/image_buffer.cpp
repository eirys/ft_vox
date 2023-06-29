/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_buffer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 09:57:49 by etran             #+#    #+#             */
/*   Updated: 2023/06/29 13:32:51 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "image_buffer.h"
#include "device.h"

#include <stdexcept> // std::runtime_error
#include <functional> // std::function

namespace scop {
namespace graphics {

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
	VkFormat format,
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
	image_info.format = format;
	image_info.tiling = tiling;
	image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	image_info.usage = usage;
	image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	image_info.samples = sample_count;
	image_info.flags = flags;

	if (vkCreateImage(device.logical_device, &image_info, nullptr, &_image) != VK_SUCCESS) {
		throw std::runtime_error("failed to create image");
	}

	// Allocate memory: check memory requirements
	VkMemoryRequirements	mem_requirements;
	vkGetImageMemoryRequirements(
		device.logical_device,
		_image,
		&mem_requirements
	);

	VkMemoryAllocateInfo	alloc_info{};
	alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	alloc_info.allocationSize = mem_requirements.size;
	alloc_info.memoryTypeIndex = device.findMemoryType(
		mem_requirements.memoryTypeBits,
		properties
	);

	if (vkAllocateMemory(device.logical_device, &alloc_info, nullptr, &_memory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate image memory");
	}

	// Bind memory to instance
	vkBindImageMemory(device.logical_device, _image, _memory, 0);
}

/**
 * @brief Create associated image view.
*/
void	ImageBuffer::initView(
	//todo
) {

}

void	ImageBuffer::destroy(Device& device) {
	vkDestroyImage(device.logical_device, _image, nullptr);
	vkFreeMemory(device.logical_device, _memory, nullptr);
	vkDestroyImageView(device.logical_device, _view, nullptr);
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
	VkImageSubresourceRange& subresource_range
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
 * @note Assuming that all images have the same size.
 * @note layer_size 
*/
void	ImageBuffer::copyFrom(
	VkCommandBuffer command_buffer,
	VkBuffer src_buffer,
	uint32_t layer_width,
	uint32_t layer_height,
	uint32_t layer_count,
	uint32_t image_count,
	uint32_t layer_size,
	uint32_t pixel_size
) {
	VkDeviceSize	image_size = layer_width * layer_height * pixel_size;
	std::vector<VkBufferImageCopy>	regions;
	regions.reserve(image_count * layer_count);

	for (uint32_t image = 0; image < image_count; image++) {
		for (uint32_t layer = 0; layer < layer_count; layer++) {
			VkBufferImageCopy	region{};
			region.bufferOffset =
				(image_size * image) +	// offset for image
				(layer_size * layer);	// offset for layer
			region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			region.imageSubresource.mipLevel = 0;
			region.imageSubresource.baseArrayLayer = image * layer_count + layer;
			region.imageSubresource.layerCount = 1;
			region.imageExtent = { layer_width, layer_height, 1 };
			regions.emplace_back(region);
		}
	}
	(void)src_buffer;
	(void)command_buffer;
}

} // namespace graphics
} // namespace vox