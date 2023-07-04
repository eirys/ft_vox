/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_buffer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 13:03:48 by etran             #+#    #+#             */
/*   Updated: 2023/07/04 09:50:20 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input_buffer.h"
// #include "engine.h"
#include "device.h"
#include "vertex.h"
#include "vector.h"
#include "command_pool.h"
#include "command_buffer.h"

#include <cstring> // memcpy

namespace scop {
namespace graphics {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void	InputBuffer::init(
	Device& device,
	CommandPool& command_pool,
	const std::vector<Vertex>& vertices,
	const std::vector<uint32_t>& indices
) {
	_createVertexBuffer(device, command_pool, vertices);
	_createIndexBuffer(device, command_pool, indices);
}

void	InputBuffer::destroy(Device& device) {
	_index_buffer.destroy(device.getLogicalDevice());
	_vertex_buffer.destroy(device.getLogicalDevice());
}

/* ========================================================================== */

const Buffer&	InputBuffer::getVertexBuffer() const noexcept {
	return _vertex_buffer;
}

const Buffer&	InputBuffer::getIndexBuffer() const noexcept {
	return _index_buffer;
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

/**
 * @brief Create the vertex buffer that'll be used to store
 * the vertices of the triangle.
*/
void	InputBuffer::_createVertexBuffer(
	Device& device,
	CommandPool& command_pool,
	const std::vector<Vertex>& vertices
) {
	const VkDeviceSize	buffer_size = sizeof(Vertex) * vertices.size();

	// Create staging buffer to upload from cpu to
	scop::graphics::Buffer	staging_buffer;
	staging_buffer.init(
		device,
		buffer_size,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
	);

	// Fill staging buffer
	staging_buffer.map(device.getLogicalDevice());
	staging_buffer.copyFrom(
		vertices.data(),
		static_cast<std::size_t>(buffer_size)
	);
	staging_buffer.unmap(device.getLogicalDevice());

	// Create vertex buffer that'll interact with gpu
	_vertex_buffer.init(
		device,
		buffer_size,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT |
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
	);

	// Transfer data from staging buffer to vertex buffer
	// VkCommandBuffer	command_buffer = beginSingleTimeCommands(
	// 	device.getLogicalDevice(),
	// 	command_pool
	// );
	CommandBuffer	command_buffer;
	command_buffer.init(device, command_pool);
	command_buffer.begin();
	_vertex_buffer.copyBuffer(
		command_buffer,
		staging_buffer,
		buffer_size
	);
	// endSingleTimeCommands(
	// 	device.getLogicalDevice(),
	// 	device.getGraphicsQueue(),
	// 	command_pool,
	// 	command_buffer
	// );
	command_buffer.end(device);
	command_buffer.destroy(device, command_pool);

	// Cleanup staging buffer
	staging_buffer.destroy(device.getLogicalDevice());
}

/**
 * @brief Create index buffer (pointers into the vertex buffer)
 */
void	InputBuffer::_createIndexBuffer(
	Device& device,
	CommandPool& command_pool,
	const std::vector<uint32_t>& indices
) {
	const VkDeviceSize	buffer_size = sizeof(uint32_t) * indices.size();

	scop::graphics::Buffer	staging_buffer;
	staging_buffer.init(
		device,
		buffer_size,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
	);

	// Fill staging buffer with indices
	staging_buffer.map(device.getLogicalDevice(), buffer_size);
	staging_buffer.copyFrom(
		indices.data(),
		static_cast<std::size_t>(buffer_size)
	);
	staging_buffer.unmap(device.getLogicalDevice());

	// Create index buffer
	_index_buffer.init(
		device,
		buffer_size,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT |
		VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
	);

	// Transfer data from staging buffer to index buffer
	// VkCommandBuffer	command_buffer = beginSingleTimeCommands(
	// 	device.getLogicalDevice(),
	// 	command_pool
	// );
	CommandBuffer	command_buffer;
	command_buffer.init(device, command_pool);
	command_buffer.begin();
	_index_buffer.copyBuffer(
		command_buffer,
		staging_buffer,
		buffer_size
	);
	// endSingleTimeCommands(
	// 	device.getLogicalDevice(),
	// 	device.getGraphicsQueue(),
	// 	command_pool,
	// 	command_buffer
	// );
	command_buffer.end(device);
	command_buffer.destroy(device, command_pool);

	// Cleanup staging buffer
	staging_buffer.destroy(device.getLogicalDevice());
}

} // namespace graphics
} // namespace scop