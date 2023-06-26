/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vertex_input.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 13:03:48 by etran             #+#    #+#             */
/*   Updated: 2023/06/23 16:40:06 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vertex_input.h"
#include "engine.h"
#include "device.h"

#include <cstring> // memcpy

namespace scop {
namespace graphics {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void	VertexInput::init(
	Device& device,
	VkCommandPool command_pool,
	const std::vector<Vertex>& vertices,
	const std::vector<uint32_t>& indices
) {
	createVertexBuffer(device, command_pool, vertices);
	createIndexBuffer(device, command_pool, indices);
}

void	VertexInput::destroy(Device& device) {
	index_buffer.destroy(device.logical_device);
	vertex_buffer.destroy(device.logical_device);
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

/**
 * Create the vertex buffer that'll be used to store the vertices of the triangle.
*/
void	VertexInput::createVertexBuffer(
	Device& device,
	VkCommandPool command_pool,
	const std::vector<Vertex>& vertices
) {
	const VkDeviceSize	buffer_size = sizeof(Vertex) * vertices.size();

	// Create staging buffer to upload from cpu to
	scop::graphics::Buffer	staging_buffer = device.createBuffer(
		buffer_size,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
	);

	// Fill staging buffer
	staging_buffer.map(device.logical_device);
	staging_buffer.copyFrom(
		vertices.data(),
		static_cast<std::size_t>(buffer_size)
	);
	staging_buffer.unmap(device.logical_device);

	// Create vertex buffer that'll interact with gpu
	vertex_buffer = device.createBuffer(
		buffer_size,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT |
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
	);

	// Transfer data from staging buffer to vertex buffer
	VkCommandBuffer	command_buffer = beginSingleTimeCommands(
		device.logical_device,
		command_pool
	);
	vertex_buffer.copyBuffer(
		command_buffer,
		staging_buffer,
		buffer_size
	);
	endSingleTimeCommands(
		device.logical_device,
		device.graphics_queue,
		command_pool,
		command_buffer
	);

	// Cleanup staging buffer
	staging_buffer.destroy(device.logical_device);
}

/**
 *  Create index buffer (pointers into the vertex buffer)
 */
void	VertexInput::createIndexBuffer(
	Device& device,
	VkCommandPool command_pool,
	const std::vector<uint32_t>& indices
) {
	const VkDeviceSize	buffer_size = sizeof(uint32_t) * indices.size();

	scop::graphics::Buffer	staging_buffer;
	staging_buffer = device.createBuffer(
		buffer_size,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | 
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
	);

	// Fill staging buffer with indices
	staging_buffer.map(device.logical_device, buffer_size);
	staging_buffer.copyFrom(
		indices.data(),
		static_cast<std::size_t>(buffer_size)
	);
	staging_buffer.unmap(device.logical_device);

	// Create index buffer
	index_buffer = device.createBuffer(
		buffer_size,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT |
		VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
	);

	// Transfer data from staging buffer to index buffer
	VkCommandBuffer	command_buffer = beginSingleTimeCommands(
		device.logical_device,
		command_pool
	);
	index_buffer.copyBuffer(
		command_buffer,
		staging_buffer,
		buffer_size
	);
	endSingleTimeCommands(
		device.logical_device,
		device.graphics_queue,
		command_pool,
		command_buffer
	);
	
	// Cleanup staging buffer
	staging_buffer.destroy(device.logical_device);
}

}  // namespace graphics
}  // namespace scop