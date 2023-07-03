/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_sampler.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 20:25:44 by etran             #+#    #+#             */
/*   Updated: 2023/07/03 22:07:52 by etran            ###   ########.fr       */
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
	const std::vector<Texture>& images
) {
	if (images.empty()) {
		throw std::invalid_argument("TextureSampler: no image provided");
	}

	_texture_count = static_cast<uint32_t>(images.size());
	_createTextureImages(device, command_pool, images);
	_createTextureImageView(device);
	_createTextureSampler(device);
}

void	TextureSampler::destroy(Device& device) {
	vkDestroySampler(device.getLogicalDevice(), _vk_texture_sampler, nullptr);
	_texture_buffer.destroy(device);
}

/* ========================================================================== */

VkSampler	TextureSampler::getTextureSampler() const noexcept {
	return _vk_texture_sampler;
}

const ImageBuffer&	TextureSampler::getTextureBuffer() const noexcept {
	return _texture_buffer;
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

/**
 * @brief Create texture images (VkImage).
 * @note Assuming all images have the same size as squares.
 * @note All textures will be stored in the same VkImage, as layers.
*/
void	TextureSampler::_createTextureImages(
	Device& device,
	VkCommandPool command_pool,
	const std::vector<Texture>& images
) {
	// Size of a side
	const uint32_t	side_size = static_cast<uint32_t>(images[0].getWidth());

	// Size of a layer (an image). A layer is a face of the cube map.
	const VkDeviceSize	layer_size = side_size * side_size * sizeof(uint32_t);

	// Size of the VKImage, including all layers
	const VkDeviceSize	image_size = layer_size * _texture_count;

	// Evaluate mip levels count for each image
	_mip_levels = 1 + static_cast<uint32_t>(
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
	for (uint32_t i = 0; i < _texture_count; ++i) {
		staging_buffer.copyFrom(
			images[i].getPixels(),
			static_cast<std::size_t>(layer_size),
			static_cast<std::size_t>(layer_size) * i
		);
	}
	staging_buffer.unmap(device.getLogicalDevice());

	// Create texture image to be filled
	_texture_buffer.initImage(
		device,
		side_size,
		side_size,
		VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_USAGE_TRANSFER_SRC_BIT |	// For mipmaps generation
		VK_IMAGE_USAGE_TRANSFER_DST_BIT |	// For copy command
		VK_IMAGE_USAGE_SAMPLED_BIT,			// For shader access
		VK_SAMPLE_COUNT_1_BIT,
		_mip_levels,
		_texture_count,
		0,
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
	transfer_barrier.baseArrayLayer = 0;
	transfer_barrier.levelCount = _mip_levels;
	transfer_barrier.layerCount = _texture_count;

	_texture_buffer.setLayout(
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
	_texture_buffer.copyFrom(
		command_buffer,
		staging_buffer.getBuffer(),
		side_size,
		side_size,
		_texture_count,
		_layer_count,
		static_cast<uint32_t>(layer_size)
	);

	// Dynamically generate mipmaps
	_texture_buffer.generateMipmap(
		command_buffer,
		device,
		side_size,
		side_size,
		VK_FORMAT_R8G8B8A8_SRGB,
		_mip_levels,
		_layer_count * _texture_count
	);

	// Submit and wait for transfer to be done before destroying buffer
	endSingleTimeCommands(
		device.getLogicalDevice(),
		device.getGraphicsQueue(),
		command_pool,
		command_buffer
	);

	staging_buffer.destroy(device.getLogicalDevice());
}

/**
 * @brief Create image view for texture sampler.
*/
void	TextureSampler::_createTextureImageView(
	Device& device
) {
	_texture_buffer.initView(
		device,
		VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_ASPECT_COLOR_BIT,
		VK_IMAGE_VIEW_TYPE_2D_ARRAY,
		_mip_levels,
		_layer_count * _texture_count
	);
}

/**
 * @brief Create sampler for texture sampling in shaders.
*/
void	TextureSampler::_createTextureSampler(
	Device& device
) {
	// VkPhysicalDeviceProperties	properties{};
	// vkGetPhysicalDeviceProperties(device.getPhysicalDevice(), &properties);

	VkSamplerCreateInfo	sampler_info{};
	sampler_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	sampler_info.magFilter = VK_FILTER_NEAREST;
	sampler_info.minFilter = VK_FILTER_NEAREST;
	sampler_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	sampler_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	sampler_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	sampler_info.anisotropyEnable = VK_FALSE;//VK_TRUE;
	sampler_info.maxAnisotropy = 0.0;// properties.limits.maxSamplerAnisotropy;
	sampler_info.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
	sampler_info.unnormalizedCoordinates = VK_FALSE;
	sampler_info.compareEnable = VK_FALSE;
	sampler_info.compareOp = VK_COMPARE_OP_NEVER;
	sampler_info.mipLodBias = 0.0f;
	sampler_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	sampler_info.minLod = 0.0f;
	sampler_info.maxLod = static_cast<float>(_mip_levels);

	if (vkCreateSampler(device.getLogicalDevice(), &sampler_info, nullptr, &_vk_texture_sampler) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture sampler");
	}
}

} // namespace graphics
} // namespace scop