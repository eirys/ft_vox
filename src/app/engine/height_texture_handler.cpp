/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   height_texture_handler.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 11:15:00 by etran             #+#    #+#             */
/*   Updated: 2023/09/22 17:00:36 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "height_texture_handler.h"
#include "device.h"
#include "chunk_macros.h"
#include "command_buffer.h"
#include "command_pool.h"
#include "buffer.h"

#include <stdexcept> // std::runtime_error

namespace scop::graphics {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void	HeightTextureHandler::init(Device& device) {
	super::_layer_count = RENDER_DISTANCE * RENDER_DISTANCE;
	super::_texture_count = 1;

	_createTextureImages(device);
	_createTextureImageView(device);
	_createTextureSampler(device);
}

/**
 * @brief Copies the height map data to the VkImage.
*/
void	HeightTextureHandler::copyData(
	Device& device,
	const std::vector<uint8_t>& height_map
) {
	constexpr const VkDeviceSize	layer_size = CHUNK_SIZE * CHUNK_SIZE * sizeof(uint8_t);
	const VkDeviceSize				image_size = layer_size * super::_layer_count;

	// Create staging buffer to copy images data to (cpu->gpu)
	scop::graphics::Buffer	staging_buffer;
	staging_buffer.init(
		device,
		image_size,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	// Copy every chunk data
	staging_buffer.map(device.getLogicalDevice());
	staging_buffer.copyFrom(height_map.data(), static_cast<std::size_t>(layer_size) * super::_layer_count);
	staging_buffer.unmap(device.getLogicalDevice());

	// Setup copy command buffer
	CommandBuffer	command_buffer = CommandPool::createBuffer(device);
	command_buffer.begin();

	// Transition to transfer destination layout
	VkImageSubresourceRange	transfer_barrier{};
	transfer_barrier.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	transfer_barrier.baseMipLevel = 0;
	transfer_barrier.baseArrayLayer = 0;
	transfer_barrier.levelCount = _mip_levels;
	transfer_barrier.layerCount = super::_layer_count;

	super::_texture_buffer.setLayout(
		command_buffer,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		0,
		VK_ACCESS_TRANSFER_WRITE_BIT,
		VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
		VK_PIPELINE_STAGE_TRANSFER_BIT,
		transfer_barrier);

	// Record copy command
	super::_texture_buffer.copyFrom(
		command_buffer,
		staging_buffer.getBuffer(),
		CHUNK_SIZE,
		CHUNK_SIZE,
		super::_layer_count,
		super::_texture_count,
		static_cast<uint32_t>(layer_size),
		sizeof(uint8_t));

	super::_texture_buffer.setLayout(
		command_buffer,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
		VK_ACCESS_TRANSFER_WRITE_BIT,
		VK_ACCESS_SHADER_READ_BIT,
		VK_PIPELINE_STAGE_TRANSFER_BIT,
		VK_PIPELINE_STAGE_VERTEX_SHADER_BIT,
		transfer_barrier);

	// Submit commands
	command_buffer.end(device);
	CommandPool::destroyBuffer(device, command_buffer);
	staging_buffer.destroy(device.getLogicalDevice());
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

void	HeightTextureHandler::_createTextureImages(Device& device) {
	super::_texture_buffer.initImage(
		device,
		CHUNK_SIZE,
		CHUNK_SIZE,
		VK_FORMAT_R8_UINT,
		VK_IMAGE_USAGE_TRANSFER_DST_BIT |	// For copy command
		VK_IMAGE_USAGE_SAMPLED_BIT,
		VK_SAMPLE_COUNT_1_BIT,
		super::_mip_levels,
		super::_layer_count,
		0,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
}

void	HeightTextureHandler::_createTextureImageView(Device& device) {
	super::_texture_buffer.initView(
		device,
		VK_FORMAT_R8_UINT,
		VK_IMAGE_ASPECT_COLOR_BIT,
		VK_IMAGE_VIEW_TYPE_2D_ARRAY,
		super::_mip_levels,
		super::_layer_count);
}

void	HeightTextureHandler::_createTextureSampler(Device& device) {
	VkSamplerCreateInfo	sampler{};
	sampler.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	sampler.magFilter = VK_FILTER_NEAREST;
	sampler.minFilter = VK_FILTER_NEAREST;
	sampler.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
	sampler.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	sampler.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	sampler.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	sampler.anisotropyEnable = VK_FALSE;
	sampler.maxAnisotropy = 1.0;
	sampler.compareEnable = VK_FALSE;
	sampler.compareOp = VK_COMPARE_OP_ALWAYS;
	sampler.mipLodBias = 0.0f;
	sampler.minLod = 0.0f;
	sampler.maxLod = 1.0f;
	sampler.borderColor = VK_BORDER_COLOR_INT_OPAQUE_WHITE;

	if (vkCreateSampler(device.getLogicalDevice(), &sampler, nullptr, &(super::_texture_sampler)) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture sampler");
	}
}

} // namespace scop::graphics