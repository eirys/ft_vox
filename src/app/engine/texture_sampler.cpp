/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_sampler.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 20:25:44 by etran             #+#    #+#             */
/*   Updated: 2023/07/03 16:40:20 by etran            ###   ########.fr       */
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
	vkDestroySampler(device.getLogicalDevice(), vk_texture_sampler, nullptr);
	texture_buffer.destroy(device);
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
	const uint32_t	side_size = static_cast<uint32_t>(images[0][0].getWidth());

	// Size of a layer (an image). A layer is a face of the cube map.
	const VkDeviceSize	layer_size = side_size * side_size * sizeof(uint32_t);
	// Size of the VKImage, including all layers
	const VkDeviceSize	image_size = layer_size * 6 * texture_count;

	// Evaluate mip levels count for each image
	mip_levels = 1 + static_cast<uint32_t>(
		std::floor(std::log2(side_size))
	);

	// Create staging buffer to copy images data to
	scop::graphics::Buffer	staging_buffer;
	staging_buffer.init(
		device,
		image_size,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
	);

	// Copy every face of every image data to staging buffer
	staging_buffer.map(device.getLogicalDevice());
	for (uint32_t i = 0; i < texture_count; ++i) {
		for (uint32_t j = 0; j < 6; ++j) {
			staging_buffer.copyFrom(
				images[i][j].getPixels(),
				static_cast<std::size_t>(layer_size),
				static_cast<std::size_t>(layer_size) * (i + j)
			);
		}
	}
	staging_buffer.unmap(device.getLogicalDevice());

	// Create texture image to be filled
	texture_buffer.initImage(
		device,
		side_size,
		side_size,
		VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_USAGE_TRANSFER_SRC_BIT |	// For mipmaps generation
		VK_IMAGE_USAGE_TRANSFER_DST_BIT |	// For copy command
		VK_IMAGE_USAGE_SAMPLED_BIT,			// For shader access
		VK_SAMPLE_COUNT_1_BIT,
		mip_levels,
		6 * texture_count,
		VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
	);

	// Setup copy command buffer
	VkCommandBuffer	command_buffer = beginSingleTimeCommands(
		device.getLogicalDevice(),
		command_pool
	);

	// Transition to transfer destination layout
	VkImageSubresourceRange	transfer_barrier{};
	transfer_barrier.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	transfer_barrier.baseMipLevel = 0;
	transfer_barrier.levelCount = mip_levels;
	transfer_barrier.baseArrayLayer = 0;
	transfer_barrier.layerCount = 6 * texture_count;

	texture_buffer.setLayout(
		command_buffer,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		0,
		VK_ACCESS_TRANSFER_WRITE_BIT,
		VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
		VK_PIPELINE_STAGE_TRANSFER_BIT,
		transfer_barrier
	);

	// Send copy command
	texture_buffer.copyFrom(
		command_buffer,
		staging_buffer.getBuffer(),
		side_size,
		side_size,
		texture_count,
		6,
		static_cast<uint32_t>(layer_size)
	);

	// Dynamically generate mipmaps
	texture_buffer.generateMipmap(
		command_buffer,
		device,
		side_size,
		side_size,
		VK_FORMAT_R8G8B8A8_SRGB,
		mip_levels,
		6
	);

	// Submit and wait for transfer to be done before destroying buffer
	endSingleTimeCommands(
		device.getLogicalDevice(),
		device.getGraphicsQueue(),
		command_pool,
		command_buffer,
		true
	);

	staging_buffer.destroy(device.getLogicalDevice());
}

/**
 * Same concept as swap chain image views
*/
void	TextureSampler::createTextureImageView(
	Device& device
) {
	texture_buffer.initView(
		device,
		VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_ASPECT_COLOR_BIT,
		VK_IMAGE_VIEW_TYPE_CUBE_ARRAY,
		mip_levels,
		6 * texture_count
	);
}

/**
 * Create sampler, that'll apply transformations to image when sampling
*/
void	TextureSampler::createTextureSampler(
	Device& device
) {
	VkPhysicalDeviceProperties	properties{};
	vkGetPhysicalDeviceProperties(device.getPhysicalDevice(), &properties);

	VkSamplerCreateInfo	sampler_info{};
	sampler_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	sampler_info.magFilter = VK_FILTER_NEAREST;
	sampler_info.minFilter = VK_FILTER_NEAREST;
	sampler_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	sampler_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	sampler_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	sampler_info.anisotropyEnable = VK_TRUE;
	sampler_info.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
	sampler_info.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
	sampler_info.unnormalizedCoordinates = VK_FALSE;
	sampler_info.compareEnable = VK_FALSE;
	sampler_info.compareOp = VK_COMPARE_OP_NEVER;
	sampler_info.mipLodBias = 0.0f;
	sampler_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	sampler_info.minLod = 0.0f;
	sampler_info.maxLod = static_cast<float>(mip_levels);

	if (vkCreateSampler(device.getLogicalDevice(), &sampler_info, nullptr, &vk_texture_sampler) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture sampler");
	}
}

} // namespace graphics
} // namespace scop