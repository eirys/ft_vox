/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 13:03:48 by etran             #+#    #+#             */
/*   Updated: 2024/01/08 15:18:05 by etran            ###   ########.fr       */
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

InputHandler::InputHandler() {
	_chunk_texture = std::make_shared<ChunkTextureHandler>();
}

/* ========================================================================== */

void	InputHandler::init(core::Device& device, const vox::GameState& game) {
	// Create chunk map data
	std::shared_ptr<ChunkTextureHandler>	chunk_texture =
		std::dynamic_pointer_cast<ChunkTextureHandler>(_chunk_texture);
	chunk_texture->init(device);
	chunk_texture->copyData(device, game.getWorld().getWorldData());

	SCOP_DEBUG("Create frustum buffer...");
	_frustum_buffer.init(
		device,
		(VkDeviceSize)InputBufferSize::Frustum,
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	_frustum_buffer.map(device, (VkDeviceSize)InputBufferSize::Frustum);
	SCOP_DEBUG("Frustum buffer created");

	SCOP_DEBUG("Create quad count buffer...");
	_quad_count_buffer.init(
		device,
		(VkDeviceSize)InputBufferSize::QuadCount,
		VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	_quad_count_buffer.map(device, (VkDeviceSize)InputBufferSize::QuadCount);
	SCOP_DEBUG("Quad count buffer created");

	SCOP_DEBUG("Create vertices data buffer...");
	_vertices_data_buffer.init(
		device,
		(VkDeviceSize)InputBufferSize::VerticesData,
		VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	SCOP_DEBUG("Vertices data buffer created");
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
	SCOP_DEBUG("Retrieving data");

// Retrieve quad count
	uint32_t quad_counts[MAX_RENDER_DISTANCE];
	_quad_count_buffer.copyTo(quad_counts, (uint32_t)InputBufferSize::QuadCount);
	for (uint32_t i = 0; i < MAX_RENDER_DISTANCE; ++i) {
		_instances_count += quad_counts[i];
	}

	SCOP_DEBUG("Quad count retrieved");
}

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