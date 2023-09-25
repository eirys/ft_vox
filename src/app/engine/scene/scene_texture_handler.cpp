/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_texture_handler.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 22:32:26 by etran             #+#    #+#             */
/*   Updated: 2023/09/22 16:38:57 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene_texture_handler.h"
#include "device.h"
#include "image_handler.h"
#include "command_pool.h"
#include "command_buffer.h"
#include "buffer.h"
#include "utils.h"
#include "ppm_loader.h"
#include "mtl_parser.h"

#include <stdexcept> // std::invalid_argument
#include <algorithm> // std::max
#include <stdexcept> // std::runtime_error
#include <cassert> // assert

namespace scop::graphics {

using Texture = SceneTextureHandler::Texture;

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void	SceneTextureHandler::init(Device& device) {
	super::_layer_count = 1;
	_createTextureImages(device);
	_createTextureImageView(device);
	_createTextureSampler(device);
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

/**
 * @brief Create texture images (VkImage).
 * @note All textures will be stored in the same VkImage, as layers.
*/
void	SceneTextureHandler::_createTextureImages(Device& device) {
	std::vector<Texture>	images = _loadTextures();

	super::_texture_count = static_cast<uint32_t>(images.size());
	const uint32_t	image_width = static_cast<uint32_t>(images[0].getWidth());
	// A layer is an image.
	const VkDeviceSize	layer_size = image_width * image_width * sizeof(uint32_t);
	// Size of the VkImage, including all layers.
	const VkDeviceSize	image_size = layer_size * super::_texture_count;

	_mip_levels = super::_getMipLevelCount(image_width);

	// Create staging buffer to copy images data to
	scop::graphics::Buffer	staging_buffer;
	staging_buffer.init(
		device,
		image_size,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	// Copy every face of every image data to staging buffer
	staging_buffer.map(device.getLogicalDevice());
	for (uint32_t i = 0; i < super::_texture_count; ++i) {
		staging_buffer.copyFrom(
			images[i].getPixels(),
			static_cast<std::size_t>(layer_size),
			static_cast<std::size_t>(layer_size) * i);
	}
	staging_buffer.unmap(device.getLogicalDevice());

	// Create texture image to be filled
	super::_texture_buffer.initImage(
		device,
		image_width,
		image_width,
		VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_USAGE_TRANSFER_SRC_BIT |	// For mipmaps generation
		VK_IMAGE_USAGE_TRANSFER_DST_BIT |	// For copy command
		VK_IMAGE_USAGE_SAMPLED_BIT,			// For shader access
		VK_SAMPLE_COUNT_1_BIT,
		super::_mip_levels,
		super::_texture_count,
		0,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	// Setup copy command buffer
	CommandBuffer	command_buffer = CommandPool::createBuffer(device);
	command_buffer.begin();

	// Transition to transfer destination layout
	VkImageSubresourceRange	transfer_barrier{};
	transfer_barrier.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	transfer_barrier.baseMipLevel = 0;
	transfer_barrier.baseArrayLayer = 0;
	transfer_barrier.levelCount = _mip_levels;
	transfer_barrier.layerCount = super::_texture_count;

	super::_texture_buffer.setLayout(
		command_buffer,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		0,
		VK_ACCESS_TRANSFER_WRITE_BIT,
		VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
		VK_PIPELINE_STAGE_TRANSFER_BIT,
		transfer_barrier);

	// Send copy command
	super::_texture_buffer.copyFrom(
		command_buffer,
		staging_buffer.getBuffer(),
		image_width,
		image_width,
		super::_texture_count,
		super::_layer_count,
		static_cast<uint32_t>(layer_size));

	// Dynamically generate mipmaps
	super::_texture_buffer.generateMipmap(
		command_buffer,
		device,
		image_width,
		image_width,
		VK_FORMAT_R8G8B8A8_SRGB,
		_mip_levels,
		super::_layer_count * super::_texture_count);

	// Submit commands
	command_buffer.end(device);
	CommandPool::destroyBuffer(device, command_buffer);
	staging_buffer.destroy(device.getLogicalDevice());
}

/**
 * @brief Create image view for sampler.
*/
void	SceneTextureHandler::_createTextureImageView(Device& device) {
	super::_texture_buffer.initView(
		device,
		VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_ASPECT_COLOR_BIT,
		VK_IMAGE_VIEW_TYPE_2D_ARRAY,
		_mip_levels,
		super::_layer_count * super::_texture_count);
}

/**
 * @brief Create sampler for texture sampling in shaders.
*/
void	SceneTextureHandler::_createTextureSampler(Device& device) {
	VkSamplerCreateInfo	sampler_info{};
	sampler_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	sampler_info.magFilter = VK_FILTER_NEAREST;
	sampler_info.minFilter = VK_FILTER_NEAREST;
	sampler_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	sampler_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	sampler_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	sampler_info.anisotropyEnable = VK_FALSE; // If true, causes linear filtering
	sampler_info.maxAnisotropy = 0;
	sampler_info.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
	sampler_info.unnormalizedCoordinates = VK_FALSE;
	sampler_info.compareEnable = VK_FALSE;
	sampler_info.compareOp = VK_COMPARE_OP_NEVER;
	sampler_info.mipLodBias = 0.0f;
	sampler_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	sampler_info.minLod = 0.0f;
	sampler_info.maxLod = static_cast<float>(_mip_levels);

	if (vkCreateSampler(device.getLogicalDevice(), &sampler_info, nullptr, &(super::_texture_sampler)) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture sampler");
	}
}

/**
 * @brief Loads images (textures).
 * @note Assuming all images have the same size as squares.
*/
std::vector<Texture>	SceneTextureHandler::_loadTextures() const {
	LOG("Loading textures...");
	std::vector<Texture>	images;
	const std::vector<std::string>	paths {
		SCOP_TEXTURE_PATH "grass_top.ppm",
		SCOP_TEXTURE_PATH "dirt.ppm",
		SCOP_TEXTURE_PATH "grass_side.ppm" };
	images.reserve(paths.size());

	assert(paths.size() < TEXTURE_SAMPLER_COUNT);

	std::optional<std::size_t> side;
	for (const std::string& path: paths) {
		scop::PpmLoader	loader(path);

		const Texture& image = images.emplace_back(loader.load());

		// Check dimensions
		if (!side.has_value()) {
			side.emplace(image.getWidth());
		} else {
			if (
				side.value() != image.getWidth() &&
				side.value() != image.getHeight()
			) {
				throw std::invalid_argument("Texture dimensions do not match");
			}
		}
	}
	LOG("Textures loaded.");

	return images;
}

} // namespace scop::graphics