/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_texture_handler.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 17:13:30 by etran             #+#    #+#             */
/*   Updated: 2023/11/10 15:58:38 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map_texture_handler.h"
#include "device.h"
#include "buffer.h"
#include "command_buffer.h"
#include "command_pool.h"

#include "chunk_macros.h"

namespace scop::graphics {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void	MapTextureHandler::init(Device& device) {
	ImageBuffer::ImageMetaData	data{};
	data.format = VK_FORMAT_R16_UINT;
	data.layer_count = 1;
	data.width = RENDER_DISTANCE;
	data.height = RENDER_DISTANCE;
	// data.height = 1;

	super::_texture_buffer.setMetaData(data);

	_createTextureImages(device);
	_createTextureImageView(device);

	CommandBuffer	command_buffer = CommandPool::createBuffer(device);
	command_buffer.begin();

	VkImageSubresourceRange	transfer_barrier{};
	transfer_barrier.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	transfer_barrier.baseMipLevel = 0;
	transfer_barrier.baseArrayLayer = 0;
	transfer_barrier.levelCount = data.getMipCount();
	transfer_barrier.layerCount = data.getLayerCount();

	super::_texture_buffer.setLayout(
		command_buffer,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
		VK_ACCESS_NONE,
		VK_ACCESS_SHADER_READ_BIT,
		VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
		VK_PIPELINE_STAGE_VERTEX_SHADER_BIT,
		transfer_barrier);

	command_buffer.end(device);
	CommandPool::destroyBuffer(device, command_buffer);
}

void	MapTextureHandler::destroy(Device& device) {
	super::_texture_buffer.destroy(device);
}

/* ========================================================================== */

void	MapTextureHandler::copyData(
	Device& device,
	const std::array<uint16_t, RENDER_DISTANCE2>& chunk_map
) {
	const ImageBuffer::ImageMetaData&	image_data = super::_texture_buffer.getMetaData();

	constexpr const VkDeviceSize		layer_size = RENDER_DISTANCE2 * sizeof(uint16_t);
	const VkDeviceSize					image_size = layer_size * image_data.getLayerCount();

	scop::graphics::Buffer	staging_buffer;
	staging_buffer.init(
		device,
		image_size,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	staging_buffer.map(device.getLogicalDevice());
	staging_buffer.copyFrom(chunk_map.data(), static_cast<std::size_t>(image_size));
	staging_buffer.unmap(device.getLogicalDevice());

	CommandBuffer	command_buffer = CommandPool::createBuffer(device);
	command_buffer.begin();

	VkImageSubresourceRange	transfer_barrier{};
	transfer_barrier.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	transfer_barrier.baseMipLevel = 0;
	transfer_barrier.baseArrayLayer = 0;
	transfer_barrier.levelCount = image_data.getMipCount();
	transfer_barrier.layerCount = image_data.getLayerCount();

	super::_texture_buffer.setLayout(
		command_buffer,
		VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		VK_ACCESS_SHADER_READ_BIT,
		VK_ACCESS_TRANSFER_WRITE_BIT,
		VK_PIPELINE_STAGE_VERTEX_SHADER_BIT,
		VK_PIPELINE_STAGE_TRANSFER_BIT,
		transfer_barrier);

	super::_texture_buffer.copyFrom(
		command_buffer,
		staging_buffer.getBuffer(),
		static_cast<uint32_t>(layer_size));

	super::_texture_buffer.setLayout(
		command_buffer,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
		VK_ACCESS_TRANSFER_WRITE_BIT,
		VK_ACCESS_SHADER_READ_BIT,
		VK_PIPELINE_STAGE_TRANSFER_BIT,
		VK_PIPELINE_STAGE_VERTEX_SHADER_BIT,
		transfer_barrier);

	command_buffer.end(device);
	CommandPool::destroyBuffer(device, command_buffer);
	staging_buffer.destroy(device.getLogicalDevice());
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

void	MapTextureHandler::_createTextureImages(Device& device) {
	VkImageCreateFlags flags = 0;

	super::_texture_buffer.initImage(
		device,
		VK_IMAGE_USAGE_TRANSFER_DST_BIT |	// For copy command
		VK_IMAGE_USAGE_SAMPLED_BIT,
		VK_SAMPLE_COUNT_1_BIT,
		flags,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_TYPE_2D);
}

void	MapTextureHandler::_createTextureImageView(Device& device) {
	super::_texture_buffer.initView(
		device,
		VK_IMAGE_ASPECT_COLOR_BIT,
		VK_IMAGE_VIEW_TYPE_2D);
}

} // namespace scop::graphics