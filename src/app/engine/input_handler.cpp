/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 13:03:48 by etran             #+#    #+#             */
/*   Updated: 2023/12/26 19:32:40 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input_handler.h"
#include "game_state.h"
#include "chunk.h"

#include "chunk_texture_handler.h"
#include "buffer.h"
// #include "culling_texture_handler.h"

#include "utils.h"

namespace scop {

using ChunkTextureHandler = gfx::ChunkTextureHandler;
// using CullingTextureHandler = gfx::CullingTextureHandler;

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void	InputHandler::init(core::Device& device, const vox::GameState& game) {
	_chunk_texture = std::make_shared<ChunkTextureHandler>();

	// Create chunk map data
	std::shared_ptr<ChunkTextureHandler>	chunk_handler =
		std::dynamic_pointer_cast<ChunkTextureHandler>(_chunk_texture);
	chunk_handler->init(device);
	chunk_handler->copyData(device, game.getWorld().getWorldData());

}

void	InputHandler::destroy(core::Device& device) {
	_chunk_texture->destroy(device);
}

/* ========================================================================== */

/**
 * @brief Update input data for compute pipelines.
*/
void	InputHandler::updateData(
	core::Device& device,
	const gfx::BoundingFrustum::Camera& camera,
	const vox::World& world
) {
// update bf
	_frustum = gfx::BoundingFrustum(camera);
	// _frustum_buffer.copyFrom(&_frustum, sizeof(gfx::BoundingFrustum));
	_input_buffer.copyFrom(&_frustum, sizeof(gfx::BoundingFrustum), (uint32_t)InputBufferOffset::Frustum);

// update chunkmap

}

/**
 * @brief Retrieve data to be used by rendering pipelines.
*/
void	InputHandler::retrieveData() {
	// Retrieve quad count
	// _quad_count_buffer.copyTo(&_instances_count, sizeof(uint32_t));
	_input_buffer.copyTo(&_instances_count, sizeof(uint32_t), (uint32_t)InputBufferOffset::QuadCount);
}

// void	InputHandler::updateVisibleChunks(
// 	core::Device& device,
// 	const gfx::BoundingFrustum::Camera& camera,
// 	const vox::World& world
// ) {
// 	std::array<uint16_t, RENDER_DISTANCE2> packed_visible_chunks{};
// 	_frustum = gfx::BoundingFrustum(camera);
// 	_instances_count = 0;

// 	const uint16_t render_distance = static_cast<uint16_t>(world.getRenderDistance());

// 	for (uint16_t z = 0; z < render_distance; ++z) {
// 		for (uint16_t x = 0; x < render_distance; ++x) {
// 			if (world.getChunk(x, 0, z).isVisible(_frustum)) {
// 				uint16_t value = z * render_distance + x;
// 				packed_visible_chunks[_instances_count] = value;
// 				++_instances_count;
// 			}
// 		}
// 	}
// 	// ++_instances_count;

// 	std::shared_ptr<CullingTextureHandler>	chunk_map =
// 		std::dynamic_pointer_cast<CullingTextureHandler>(_chunk_map);
// 	chunk_map->copyData(device, packed_visible_chunks);
// }

/* ========================================================================== */

uint32_t	InputHandler::getVerticesCount() const noexcept {
	return _vertices_count;
}

uint32_t	InputHandler::getInstancesCount() const noexcept {
	return _instances_count;
}

InputHandler::TextureHandlerPtr	InputHandler::getChunkMap() const {
	return _chunk_texture;
}

// const gfx::Buffer&	InputHandler::getFrustumBuffer() const noexcept {
// 	return _frustum_buffer;
// }

// gfx::Buffer&	InputHandler::getFrustumBuffer() noexcept {
// 	return _frustum_buffer;
// }

// const gfx::Buffer&	InputHandler::getQuadCountBuffer() const noexcept {
// 	return _quad_count_buffer;
// }

// gfx::Buffer&	InputHandler::getQuadCountBuffer() noexcept {
// 	return _quad_count_buffer;
// }

// const gfx::Buffer&	InputHandler::getVerticesDataBuffer() const noexcept {
// 	return _vertices_data_buffer;
// }

// gfx::Buffer&	InputHandler::getVerticesDataBuffer() noexcept {
// 	return _vertices_data_buffer;
// }

const gfx::Buffer&	InputHandler::getInputBuffer() const noexcept {
	return _input_buffer;
}

gfx::Buffer&	InputHandler::getInputBuffer() noexcept {
	return _input_buffer;
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

void	InputHandler::_createBuffers(core::Device& device) {
	std::vector<VkBufferCreateInfo>	buffer_infos;
	buffer_infos.reserve(3);

	{
		VkBufferCreateInfo	frustum_info{};
		frustum_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		frustum_info.size = (uint32_t)InputBufferSize::Frustum;
		frustum_info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		frustum_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		buffer_infos.emplace_back(frustum_info);
	}
	{
		VkBufferCreateInfo	quad_count_info{};
		quad_count_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		quad_count_info.size = (uint32_t)InputBufferSize::QuadCount;
		quad_count_info.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
		quad_count_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		buffer_infos.emplace_back(quad_count_info);
	}
	{
		VkBufferCreateInfo	vertices_data_info{};
		vertices_data_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		vertices_data_info.size = (uint32_t)InputBufferSize::VerticesData;
		vertices_data_info.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
		vertices_data_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		buffer_infos.emplace_back(vertices_data_info);
	}

	_input_buffer.init(
		device,
		buffer_infos,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	// _frustum_buffer.init(
	// 	device,
	// 	sizeof(gfx::BoundingFrustum),
	// 	VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
	// 	VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	// _quad_count_buffer.create(
	// 	device,
	// 	sizeof(uint32_t),
	// 	VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
	// 	VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	// _vertices_data_buffer.create(
	// 	device,
	// 	sizeof(gfx::Vertex) * MAX_VERTICES_COUNT,
	// 	VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
	// 	VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

} // namespace scop