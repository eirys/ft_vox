/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 13:03:48 by etran             #+#    #+#             */
/*   Updated: 2024/01/05 13:47:38 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input_handler.h"
#include "game_state.h"
#include "chunk_texture_handler.h"

#include "debug.h"

namespace scop {

using ChunkTextureHandler = gfx::ChunkTextureHandler;

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

	LOG("Create frustum buffer...");
	_frustum_buffer.init(
		device,
		(VkDeviceSize)InputBufferSize::Frustum,
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	_frustum_buffer.map(device, (VkDeviceSize)InputBufferSize::Frustum);
	LOG("Frustum buffer created");

	LOG("Create quad count buffer...");
	_quad_count_buffer.init(
		device,
		(VkDeviceSize)InputBufferSize::QuadCount,
		VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	_quad_count_buffer.map(device, (VkDeviceSize)InputBufferSize::QuadCount);
	LOG("Quad count buffer created");

	LOG("Create vertices data buffer...");
	_vertices_data_buffer.init(
		device,
		(VkDeviceSize)InputBufferSize::VerticesData,
		VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	LOG("Vertices data buffer created");
}

void	InputHandler::destroy(core::Device& device) {
	_chunk_texture->destroy(device);
	_frustum_buffer.destroy(device);
	_quad_count_buffer.destroy(device);
	_vertices_data_buffer.destroy(device);
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
	(void)device, (void)world;
// update bf
	_frustum = gfx::BoundingFrustum(camera);
	_frustum_buffer.copyFrom(&_frustum, sizeof(gfx::BoundingFrustum), (uint32_t)InputBufferOffset::Frustum);

// update chunkmap

}

/**
 * @brief Retrieve data to be used by rendering pipelines.
*/
void	InputHandler::retrieveData() {
	// Retrieve quad count
	LOG("Retrieving data");

	uint32_t quad_counts[MAX_RENDER_DISTANCE];
	_quad_count_buffer.copyTo(quad_counts, (uint32_t)InputBufferSize::QuadCount);
	for (uint32_t i = 0; i < MAX_RENDER_DISTANCE; ++i) {
		_instances_count += quad_counts[i];
	}

	LOG("Quad count retrieved");
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

const gfx::Buffer&	InputHandler::getFrustumBuffer() const noexcept {
	return _frustum_buffer;
}

gfx::Buffer&	InputHandler::getFrustumBuffer() noexcept {
	return _frustum_buffer;
}

const gfx::Buffer&	InputHandler::getQuadCountBuffer() const noexcept {
	return _quad_count_buffer;
}

gfx::Buffer&	InputHandler::getQuadCountBuffer() noexcept {
	return _quad_count_buffer;
}

const gfx::Buffer&	InputHandler::getVerticesDataBuffer() const noexcept {
	return _vertices_data_buffer;
}

gfx::Buffer&	InputHandler::getVerticesDataBuffer() noexcept {
	return _vertices_data_buffer;
}

} // namespace scop