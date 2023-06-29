/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_sampler.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 20:25:44 by etran             #+#    #+#             */
/*   Updated: 2023/06/29 22:38:47 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "texture_sampler.h"
#include "engine.h"
#include "image_handler.h"
#include "device.h"
#include "utils.h"
#include "buffer.h"

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
	const std::vector<CubeMap>& images
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
	const std::vector<CubeMap>& images
) {
	// Size of a side
	constexpr const std::size_t	side_size = 16;
	// A layer = a face of the cube map
	const std::size_t	layer_count = 6 * images.size();

	// Size of a layer (an image). A layer is a face of the cube map.
	const VkDeviceSize	layer_size = side_size * side_size * sizeof(uint32_t);
	// Size of the VKImage, including all layers
	const VkDeviceSize	image_size = layer_size * layer_count;

	// Evaluate mip levels count for each image
	mip_levels = 1 + static_cast<uint32_t>(
		std::floor(std::log2(side_size))
	);

	// Create staging buffer to copy images data to
	scop::graphics::Buffer	staging_buffer = device.createBuffer(
		image_size,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
	);

	// Copy every face of every image data to staging buffer
	staging_buffer.map(device.logical_device);
	for (std::size_t i = 0; i < images.size(); ++i) {
		for (std::size_t j = 0; j < 6; ++j) {
			staging_buffer.copyFrom(
				images[i][j].getPixels(),
				static_cast<std::size_t>(layer_size),
				static_cast<std::size_t>(layer_size) * (i + j)
			);
		}
	}
	staging_buffer.unmap(device.logical_device);

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

	// Send copy command
	copyBufferToImage(
		command_buffer,
		staging_buffer.getBuffer(),
		vk_texture_image,
		static_cast<uint32_t>(side_size),
		sizeof(uint32_t),
		static_cast<uint32_t>(texture_count),
		static_cast<uint32_t>(mip_levels)
	);

	// Submit to graphics queue to execute transfer
	endSingleTimeCommands(
		device.logical_device,
		device.graphics_queue,
		command_pool,
		command_buffer,
		false
	);

	// Fill mipmaps images (directly handled by gpu)
	generateMipmaps(
		command_buffer,
		device,
		vk_texture_image,
		VK_FORMAT_R8G8B8A8_SRGB,
		static_cast<int32_t>(side_size),
		static_cast<uint32_t>(mip_levels),
		static_cast<uint32_t>(layer_count)
	);

	// Submit and wait for transfer to be done before destroying buffer
	endSingleTimeCommands(
		device.logical_device,
		device.graphics_queue,
		command_pool,
		command_buffer,
		true
	);

	staging_buffer.destroy(device.logical_device);
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
	sampler_info.mipLodBias = 0.0f;
	sampler_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	sampler_info.minLod = 0.0f;
	sampler_info.maxLod = static_cast<float>(mip_levels);

	if (vkCreateSampler(device.logical_device, &sampler_info, nullptr, &vk_texture_sampler) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture sampler");
	}
}

void	TextureSampler::generateMipmaps(
	VkCommandBuffer buffer,
	Device& device,
	VkImage image,
	VkFormat image_format,
	int32_t face_size,
	uint32_t mip_level_count,
	uint32_t layer_count
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

	for (uint32_t layer = 0; layer < layer_count ; ++layer) {
		for (uint32_t level = 1; level < mip_level_count; ++level) {
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			barrier.subresourceRange.baseMipLevel = level - 1;
			barrier.subresourceRange.levelCount = 1;
			barrier.subresourceRange.baseArrayLayer = layer;
			barrier.subresourceRange.layerCount = 1;

			VkImageBlit	blit{};

			// Source
			blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.srcSubresource.baseArrayLayer = layer;
			blit.srcSubresource.mipLevel = level - 1;
			blit.srcSubresource.layerCount = 1;
			blit.srcOffsets[0] = { 0, 0, 0 };
			blit.srcOffsets[1].x = face_size >> (level - 1);
			blit.srcOffsets[1].y = face_size >> (level - 1);
			blit.srcOffsets[1].z = 1;

			// Destination
			blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.dstSubresource.baseArrayLayer = layer;
			blit.dstSubresource.mipLevel = level;
			blit.dstSubresource.layerCount = 1;
			blit.dstOffsets[0] = { 0, 0, 0 };
			blit.dstOffsets[1].x = face_size >> level;
			blit.dstOffsets[1].y = face_size >> level;
			blit.dstOffsets[1].z = 1;

			// Transition previous mip level to transfer source
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

			// Blit image
			vkCmdBlitImage(
				buffer,
				image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1, &blit,
				VK_FILTER_LINEAR
			);

			// Prepare for next level
			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			vkCmdPipelineBarrier(
				buffer,
				VK_PIPELINE_STAGE_TRANSFER_BIT,
				VK_PIPELINE_STAGE_TRANSFER_BIT,
				0,
				0, nullptr,
				0, nullptr,
				1, &barrier
			);
		}
	}
	// Revert layout all level to shader read
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = layer_count;
	barrier.subresourceRange.levelCount = mip_level_count;
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