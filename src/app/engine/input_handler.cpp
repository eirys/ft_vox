/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 13:03:48 by etran             #+#    #+#             */
/*   Updated: 2023/10/29 02:33:25 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input_handler.h"
#include "game_state.h"
#include "chunk.h"
#include "matrix.h"
#include "chunk_texture_handler.h"

#include "utils.h"

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

void	InputHandler::updateVisibleChunks(
	std::array<uint32_t, PACKED_DATA_COUNT>& packed_visible_chunks,
	const BoundingFrustum::Camera& camera,
	const ::vox::World& world
) {
	_frustum = BoundingFrustum(camera);
	_instances_count = 0;

	for (uint16_t z = 0; z < world.getRenderDistance(); ++z) {
		for (uint16_t x = 0; x < world.getRenderDistance(); ++x) {

			if (world.getChunk(x, 0, z).isVisible(_frustum)) {

				uint32_t value = static_cast<uint32_t>(z * world.getRenderDistance() + x);
				if (_instances_count % 2 == 0)
					value >>= 16;

				packed_visible_chunks[_instances_count / 2] |= value;
				++_instances_count;
			}
		}
	}
}

/* ========================================================================== */

uint32_t	InputHandler::getVerticesCount() const noexcept {
	return _vertices_count;
}

uint32_t	InputHandler::getInstancesCount() const noexcept {
	return CHUNK_AREA * _instances_count;
}

InputHandler::TextureHandlerPtr	InputHandler::getHeightMap() const noexcept {
	return _height_map;
}

} // namespace scop::graphics