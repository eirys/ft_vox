/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 13:03:48 by etran             #+#    #+#             */
/*   Updated: 2023/08/15 19:25:57 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input_handler.h"
#include "device.h"
#include "vertex.h"
#include "vector.h"
#include "command_pool.h"
#include "command_buffer.h"

#include <cstring> // memcpy

namespace scop::graphics {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void	InputHandler::init(
	Device& device,
	const std::vector<Vertex>& vertices,
	const std::vector<uint32_t>& indices
) {
	_indices_count = indices.size();
	_createVertexBuffer(device, vertices);
	_createIndexBuffer(device, indices);
}

void	InputHandler::destroy(Device& device) {
	_index_buffer.destroy(device.getLogicalDevice());
	_vertex_buffer.destroy(device.getLogicalDevice());
}

/* ========================================================================== */

uint32_t	InputHandler::getIndicesCount() const noexcept {
	return _indices_count;
}

const Buffer&	InputHandler::getVertexBuffer() const noexcept {
	return _vertex_buffer;
}

const Buffer&	InputHandler::getIndexBuffer() const noexcept {
	return _index_buffer;
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

/**
 * @brief Create the vertex buffer that'll be used to store
 * the vertices of the triangle.
*/
void	InputHandler::_createVertexBuffer(
	Device& device,
	const std::vector<Vertex>& vertices
) {
	const VkDeviceSize	buffer_size = sizeof(Vertex) * vertices.size();

	// Create staging buffer to upload to from cpu
	scop::graphics::Buffer	staging_buffer;
	staging_buffer.init(
		device,
		buffer_size,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	// Fill staging buffer
	staging_buffer.map(device.getLogicalDevice());
	staging_buffer.copyFrom(
		vertices.data(),
		static_cast<std::size_t>(buffer_size));
	staging_buffer.unmap(device.getLogicalDevice());

	// Create vertex buffer
	_vertex_buffer.init(
		device,
		buffer_size,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT |
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	// Transfer data from staging buffer to vertex buffer
	CommandBuffer	command_buffer = CommandPool::createBuffer(device);
	command_buffer.begin();
	_vertex_buffer.copyBuffer(
		command_buffer,
		staging_buffer,
		buffer_size);
	command_buffer.end(device);
	CommandPool::destroyBuffer(device, command_buffer);

	// Cleanup staging buffer
	staging_buffer.destroy(device.getLogicalDevice());
}

/**
 * @brief Create index buffer (pointers into the vertex buffer)
 */
void	InputHandler::_createIndexBuffer(
	Device& device,
	const std::vector<uint32_t>& indices
) {
	const VkDeviceSize	buffer_size = sizeof(uint32_t) * indices.size();

	scop::graphics::Buffer	staging_buffer;
	staging_buffer.init(
		device,
		buffer_size,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	// Fill staging buffer with indices
	staging_buffer.map(device.getLogicalDevice(), buffer_size);
	staging_buffer.copyFrom(
		indices.data(),
		static_cast<std::size_t>(buffer_size));
	staging_buffer.unmap(device.getLogicalDevice());

	// Create index buffer
	_index_buffer.init(
		device,
		buffer_size,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT |
		VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	// Transfer data from staging buffer to index buffer
	CommandBuffer	command_buffer = CommandPool::createBuffer(device);
	command_buffer.begin();
	_index_buffer.copyBuffer(
		command_buffer,
		staging_buffer,
		buffer_size);
	command_buffer.end(device);
	CommandPool::destroyBuffer(device, command_buffer);

	// Cleanup staging buffer
	staging_buffer.destroy(device.getLogicalDevice());
}

} // namespace scop::graphics