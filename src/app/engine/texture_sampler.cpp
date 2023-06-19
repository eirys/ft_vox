/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_sampler.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 20:25:44 by etran             #+#    #+#             */
/*   Updated: 2023/06/19 09:45:28 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "texture_sampler.h"
#include "engine.h"
#include "image_handler.h"
#include "device.h"
#include "utils.h"

#include <cmath> // std::floor
#include <algorithm> // std::max
#include <stdexcept> // std::runtime_error
#include <cstring> // memcpy

namespace scop {
namespace graphics {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void	TextureSampler::init(
	Device& device,
	VkCommandPool command_pool,
	const std::vector<scop::Image>& images
) {
	texture_count = static_cast<uint32_t>(images.size());
	createTextureImages(device, command_pool, images);
	createTextureImageView(device);
	createTextureSampler(device);
}

void	TextureSampler::destroy(Device& device) {
	vkDestroySampler(device.logical_device, vk_texture_sampler, nullptr);
	vkDestroyImageView(device.logical_device, vk_texture_image_view, nullptr);

	vkDestroyImage(device.logical_device, vk_texture_image, nullptr);
	vkFreeMemory(device.logical_device, vk_texture_image_memory, nullptr);
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

/**
 * @brief Create texture images (VkImage).
 * Since they all have the same size (16 * 16), we optimize the process by creating
 * a single staging buffer and a single VkImage.
*/
void	TextureSampler::createTextureImages(
	Device& device,
	VkCommandPool command_pool,
	const std::vector<scop::Image>& images
) {
	// Retrieve data from image vector
	const std::size_t	side_size = images[0].getWidth();
	// A layer = a cube map face
	const std::size_t	layer_count = 6 * texture_count;

	// Evaluate mip levels count for each image
	mip_levels = 1 + static_cast<uint32_t>(
		std::floor(std::log2(side_size))
	);

	// Create single staging buffer
	VkBuffer		staging_buffer;
	VkDeviceMemory	staging_buffer_memory;
	VkDeviceSize	image_size = side_size * side_size * sizeof(uint32_t);

	device.createBuffer(
		image_size,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		staging_buffer,
		staging_buffer_memory
	);

	// Copy every images data to staging buffer
	void*		data;
	vkMapMemory(device.logical_device, staging_buffer_memory, 0, image_size, 0, &data);
	for (std::size_t i = 0; i < texture_count; ++i) {
		std::size_t	offset = static_cast<std::size_t>(image_size) * i;
		memcpy(
			static_cast<char*>(data) + offset,
			images[i].getPixels(),
			static_cast<std::size_t>(image_size)
		);
	}
	vkUnmapMemory(device.logical_device, staging_buffer_memory);

	// Create texture image to be filled
	device.createImage(
		side_size,
		side_size,
		mip_levels,
		layer_count,
		VK_SAMPLE_COUNT_1_BIT,
		VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_TRANSFER_SRC_BIT |	// For mipmaps generation
		VK_IMAGE_USAGE_TRANSFER_DST_BIT |	// For copy command
		VK_IMAGE_USAGE_SAMPLED_BIT,			// For shader access
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT,
		vk_texture_image,
		vk_texture_image_memory
	);

	// Setup copy command buffer
	VkCommandBuffer	command_buffer = beginSingleTimeCommands(
		device.logical_device,
		command_pool
	);
	// // Set image layout to be optimal for transfer
	// transitionImageLayout(
	// 	command_buffer,
	// 	device.graphics_queue,
	// 	vk_texture_image,
	// 	VK_FORMAT_R8G8B8A8_SRGB,
	// 	VK_IMAGE_LAYOUT_UNDEFINED,
	// 	VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
	// 	mip_levels
	// );

	// Send copy command
	copyBufferToImage(
		command_buffer,
		staging_buffer,
		vk_texture_image,
		static_cast<uint32_t>(side_size),
		4 * sizeof(uint8_t), // Bytes per pixel: 4 (RGBA)
		texture_count,
		mip_levels
	);

	// Submit to graphics queue to execute transfer
	vkEndCommandBuffer(command_buffer);

	// Fill mipmaps images (directly handled by gpu)
	generateMipmaps(
		command_buffer,
		device,
		vk_texture_image,
		VK_FORMAT_R8G8B8A8_SRGB,
		side_size,
		mip_levels
	);

	// Submit and wait for transfer to be done before destroying buffer
	endSingleTimeCommands(
		device.logical_device,
		device.graphics_queue,
		command_pool,
		command_buffer
	);

	vkDestroyBuffer(device.logical_device, staging_buffer, nullptr);
	vkFreeMemory(device.logical_device, staging_buffer_memory, nullptr);
}

/**
 * Same concept as swap chain image views
*/
void	TextureSampler::createTextureImageView(
	Device& device
) {
	vk_texture_image_view = createImageView(
		device.logical_device,
		vk_texture_image,
		VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_ASPECT_COLOR_BIT,
		VK_IMAGE_VIEW_TYPE_CUBE_ARRAY,
		mip_levels,
		texture_count * 6 // Nb layers = nb images * 6 (cube map)
	);
}

/**
 * Create sampler, that'll apply transformations to image when sampling
*/
void	TextureSampler::createTextureSampler(
	Device& device
) {
	VkSamplerCreateInfo	sampler_info{};
	sampler_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	sampler_info.magFilter = VK_FILTER_NEAREST;
	sampler_info.minFilter = VK_FILTER_NEAREST;
	sampler_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	sampler_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	sampler_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	sampler_info.anisotropyEnable = VK_FALSE;
	sampler_info.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
	sampler_info.unnormalizedCoordinates = VK_FALSE;
	sampler_info.compareEnable = VK_FALSE;
	sampler_info.compareOp = VK_COMPARE_OP_ALWAYS;
	sampler_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
	sampler_info.mipLodBias = 0.0f;
	sampler_info.minLod = 0.0f;
	sampler_info.maxLod = static_cast<float>(mip_levels);

	if (vkCreateSampler(device.logical_device, &sampler_info, nullptr, &vk_texture_sampler) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture sampler");
	}
}

/**
 * Record command to generate mipmaps levels
*/
// void	TextureSampler::generateMipmaps(
// 	Device& device,
// 	VkCommandPool command_pool,
// 	VkImage image,
// 	VkFormat image_format,
// 	int32_t tex_width,
// 	int32_t tex_height,
// 	uint32_t mip_level
// ) const {
// 	// Check if support blitting
// 	VkFormatProperties	properties;
// 	vkGetPhysicalDeviceFormatProperties(device.physical_device, image_format, &properties);

// 	if (!(properties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
// 		throw std::runtime_error("texture image format doesn't support linear blitting");
// 	}

// 	VkCommandBuffer	command_buffer = beginSingleTimeCommands(
// 		device.logical_device,
// 		command_pool
// 	);

// 	VkImageMemoryBarrier	barrier{};
// 	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
// 	barrier.image = image;
// 	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
// 	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
// 	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
// 	barrier.subresourceRange.baseArrayLayer = 0;
// 	barrier.subresourceRange.layerCount = 1;
// 	barrier.subresourceRange.levelCount = 1;

// 	int32_t	mip_width = tex_width;
// 	int32_t	mip_height = tex_height;

// 	// Redefine parts of barrier for each level
// 	for (uint32_t i = 1; i < mip_level; ++i) {
// 		barrier.subresourceRange.baseMipLevel = i - 1;
// 		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
// 		barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
// 		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
// 		barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

// 		vkCmdPipelineBarrier(
// 			command_buffer,
// 			VK_PIPELINE_STAGE_TRANSFER_BIT,
// 			VK_PIPELINE_STAGE_TRANSFER_BIT,
// 			0,
// 			0,
// 			nullptr,
// 			0,
// 			nullptr,
// 			1,
// 			&barrier
// 		);

// 		// Define blit region
// 		VkImageBlit	blit{};
// 		blit.srcOffsets[0] = { 0, 0, 0 };
// 		blit.srcOffsets[1] = { mip_width, mip_height, 1 };
// 		blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
// 		blit.srcSubresource.mipLevel = i - 1;
// 		blit.srcSubresource.baseArrayLayer = 0;
// 		blit.srcSubresource.layerCount = 1;
// 		blit.dstOffsets[0] = { 0, 0, 0 };
// 		blit.dstOffsets[1] = {
// 			mip_width > 1 ? mip_width / 2 : 1,
// 			mip_height > 1 ? mip_height / 2: 1,
// 			1
// 		};
// 		blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
// 		blit.dstSubresource.mipLevel = i;
// 		blit.dstSubresource.baseArrayLayer = 0;
// 		blit.dstSubresource.layerCount = 1;

// 		// Record blit command
// 		vkCmdBlitImage(
// 			command_buffer,
// 			image,
// 			VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
// 			image,
// 			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
// 			1,
// 			&blit,
// 			VK_FILTER_LINEAR
// 		);

// 		// Transition to shader readable layout
// 		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
// 		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
// 		barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
// 		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

// 		vkCmdPipelineBarrier(
// 			command_buffer,
// 			VK_PIPELINE_STAGE_TRANSFER_BIT,
// 			VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
// 			0,
// 			0,
// 			nullptr,
// 			0,
// 			nullptr,
// 			1,
// 			&barrier
// 		);

// 		// Set next mip level size
// 		if (mip_width > 1) {
// 			mip_width /= 2;
// 		}
// 		if (mip_height > 1) {
// 			mip_height /= 2;
// 		}
// 	}

// 	// Last mip level
// 	barrier.subresourceRange.baseMipLevel = mip_level - 1;
// 	barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
// 	barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
// 	barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
// 	barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

// 	vkCmdPipelineBarrier(
// 		command_buffer,
// 		VK_PIPELINE_STAGE_TRANSFER_BIT,
// 		VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
// 		0,
// 		0,
// 		nullptr,
// 		0,
// 		nullptr,
// 		1,
// 		&barrier
// 	);

// 	endSingleTimeCommands(
// 		device.logical_device,
// 		device.graphics_queue,
// 		command_pool,
// 		command_buffer
// 	);
// }

void	TextureSampler::generateMipmaps(
	VkCommandBuffer buffer,
	Device& device,
	VkImage image,
	VkFormat image_format,
	int32_t tex_side,
	uint32_t mip_level_count
) const {
	// Check if support blitting
	VkFormatProperties	properties;
	vkGetPhysicalDeviceFormatProperties(device.physical_device, image_format, &properties);

	if (!(properties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
		throw std::runtime_error("texture image format doesn't support linear blitting");
	}

	VkImageMemoryBarrier	barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.image = image;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.subresourceRange.levelCount = 1;

	int32_t	mip_side = tex_side;
	const constexpr uint32_t	face_count = 6;

	// Redefine parts of barrier for each level, for each layer, for each face
	for (uint32_t face = 0; face < face_count; ++face) {
		for (uint32_t layer = 0; layer < texture_count; ++layer) {
			for (uint32_t level = 1; level < mip_level_count; ++level) {
				barrier.subresourceRange.baseMipLevel = level - 1;
				barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

				vkCmdPipelineBarrier(
					buffer,
					VK_PIPELINE_STAGE_TRANSFER_BIT,
					VK_PIPELINE_STAGE_TRANSFER_BIT,
					0,
					0, nullptr,
					0, nullptr,
					1, &barrier
				);

				int32_t	mip_offset = mip_side > 1 ? mip_side / 2 : 1;

				// Define blit region for current level
				VkImageBlit	blit{};
				blit.srcOffsets[0] = { 0, 0, 0 };
				blit.srcOffsets[1] = { tex_side, tex_side, 1 };
				blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				blit.srcSubresource.mipLevel = level - 1;
				blit.srcSubresource.baseArrayLayer = layer * face_count + face;
				blit.srcSubresource.layerCount = 1;

				blit.dstOffsets[0] = { 0, 0, 0 };
				blit.dstOffsets[1] = { mip_offset, mip_offset, 1 };
				blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				blit.dstSubresource.mipLevel = level;
				blit.dstSubresource.baseArrayLayer = layer * face_count + face;
				blit.dstSubresource.layerCount = 1;

				// Record blit command
				vkCmdBlitImage(
					buffer,
					image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
					image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
					1, &blit,
					VK_FILTER_LINEAR
				);

				// Transition to shader readable layout
				barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
				barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
				barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

				vkCmdPipelineBarrier(
					buffer,
					VK_PIPELINE_STAGE_TRANSFER_BIT,
					VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
					0,
					0, nullptr,
					0, nullptr,
					1, &barrier
				);

				// Set next mip level size
				if (mip_side > 1) {
					mip_side /= 2;
				}
			}
		}
	}

	// Last mip level
	barrier.subresourceRange.baseMipLevel = mip_level_count - 1;
	barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

	vkCmdPipelineBarrier(
		buffer,
		VK_PIPELINE_STAGE_TRANSFER_BIT,
		VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
		0,
		0, nullptr,
		0, nullptr,
		1, &barrier
	);
}

/* ========================================================================== */
/*                                    OTHER                                   */
/* ========================================================================== */

void	transitionImageLayout(
	VkCommandBuffer buffer,
	VkImage image,
	VkFormat format,
	VkImageLayout old_layout,
	VkImageLayout new_layout,
	uint32_t mip_level
) {
	(void)format;
	// Create image memory barrier to synchronize proper access to resources
	VkImageMemoryBarrier	barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = old_layout;
	barrier.newLayout = new_layout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = image;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = mip_level;
	barrier.subresourceRange.layerCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.srcAccessMask = 0;
	barrier.dstAccessMask = 0;

	// Set access maks depending on layout in transition,
	// cause multiple actions will be performed during pipeline execution
	VkPipelineStageFlags	src_stage;
	VkPipelineStageFlags	dst_stage;

	if (
		old_layout == VK_IMAGE_LAYOUT_UNDEFINED &&
		new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
	) {
		// Transfer destination doesn't need waiting, so do it in the beginning
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		src_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		dst_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	} else if (
		old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
		new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
	) {
		// Shader reading needs to wait for transfer to finish
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		src_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		dst_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	} else {
		throw std::invalid_argument("unsupported layout transition");
	}

	// Submit barrier
	vkCmdPipelineBarrier(
		buffer,
		src_stage,
		dst_stage,
		0,
		0, nullptr,
		0, nullptr,
		1, &barrier
	);
}

} // namespace graphics
} // namespace scop