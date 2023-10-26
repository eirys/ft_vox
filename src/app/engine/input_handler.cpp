/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 13:03:48 by etran             #+#    #+#             */
/*   Updated: 2023/10/25 21:04:31 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input_handler.h"
#include "game_state.h"
#include "chunk.h"
#include "matrix.h"
#include "chunk_texture_handler.h"

namespace scop::graphics {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void	InputHandler::init(Device& device, const ::vox::GameState& game) {
	_height_map = std::make_shared<ChunkTextureHandler>();

	std::shared_ptr<ChunkTextureHandler>	height_map =
		std::dynamic_pointer_cast<ChunkTextureHandler>(_height_map);
	height_map->init(device);
	height_map->copyData(device, game.getWorld().generateHeightBuffer());
}

void	InputHandler::destroy(Device& device) {
	_height_map->destroy(device);
}

uint32_t	InputHandler::updateVisibleChunks(
	const BoundingFrustum::Camera& camera,
	const ::vox::World& world
) {
	uint32_t	packed_visible_chunks = 0;
	_instances_count = 0;

	_frustum = BoundingFrustum(camera);

	for (uint32_t z = 0; z < world.getChunkCoundDepth(); ++z) {
		for (uint32_t x = 0; x < world.getChunkCoundWidth(); ++x) {

			if (world.getChunk(x, 0, z).isVisible(_frustum)) {
				packed_visible_chunks |= 1 << (32-(z * world.getChunkCoundWidth() + x));
				++_instances_count;
			}

		}
	}
	return packed_visible_chunks;
}

/* ========================================================================== */

// TODO
uint32_t	InputHandler::getVerticesCount() const noexcept {
	return _vertices_count;
}

//TODO
uint32_t	InputHandler::getInstancesCount() const noexcept {
	return CHUNK_AREA * _instances_count;
}

InputHandler::TextureHandlerPtr	InputHandler::getHeightMap() const noexcept {
	return _height_map;
}

} // namespace scop::graphics