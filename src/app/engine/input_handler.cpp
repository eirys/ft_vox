/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 13:03:48 by etran             #+#    #+#             */
/*   Updated: 2023/11/16 22:55:39 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input_handler.h"
#include "game_state.h"
#include "chunk.h"

#include "chunk_texture_handler.h"
#include "map_texture_handler.h"

#include "utils.h"

namespace scop {

using ChunkTextureHandler = gfx::ChunkTextureHandler;
using MapTextureHandler = gfx::MapTextureHandler;

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void	InputHandler::init(core::Device& device, const vox::GameState& game) {
	_height_map = std::make_shared<ChunkTextureHandler>();
	_chunk_map = std::make_shared<MapTextureHandler>();

	std::shared_ptr<ChunkTextureHandler>	height_map =
		std::dynamic_pointer_cast<ChunkTextureHandler>(_height_map);
	height_map->init(device);
	height_map->copyData(device, game.getWorld().generateHeightBuffer());

	std::shared_ptr<MapTextureHandler>	chunk_map =
		std::dynamic_pointer_cast<MapTextureHandler>(_chunk_map);
	chunk_map->init(device);
	chunk_map->setTextureSampler(height_map->getTextureSampler());
}

void	InputHandler::destroy(core::Device& device) {
	_height_map->destroy(device);
	_chunk_map->destroy(device);
}

/* ========================================================================== */

void	InputHandler::updateVisibleChunks(
	core::Device& device,
	const gfx::BoundingFrustum::Camera& camera,
	const vox::World& world
) {
	std::array<uint16_t, RENDER_DISTANCE2> packed_visible_chunks{};
	_frustum = gfx::BoundingFrustum(camera);
	_instances_count = 0;

	const uint16_t render_distance = static_cast<uint16_t>(world.getRenderDistance());

	for (uint16_t z = 0; z < render_distance; ++z) {
		for (uint16_t x = 0; x < render_distance; ++x) {
			if (world.getChunk(x, 0, z).isVisible(_frustum)) {
				uint16_t value = z * render_distance + x;
				packed_visible_chunks[_instances_count] = value;
				++_instances_count;
			}
		}
	}
	++_instances_count;

	std::shared_ptr<MapTextureHandler>	chunk_map =
		std::dynamic_pointer_cast<MapTextureHandler>(_chunk_map);
	chunk_map->copyData(device, packed_visible_chunks);
}

/* ========================================================================== */

uint32_t	InputHandler::getVerticesCount() const noexcept {
	return _vertices_count;
}

uint32_t	InputHandler::getInstancesCount() const noexcept {
	return _instances_count;
}

InputHandler::TextureHandlerPtr	InputHandler::getHeightMap() const noexcept {
	return _height_map;
}

InputHandler::TextureHandlerPtr	InputHandler::getChunkMap() const noexcept {
	return _chunk_map;
}

} // namespace scop