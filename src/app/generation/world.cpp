/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 17:42:36 by etran             #+#    #+#             */
/*   Updated: 2023/10/06 12:58:17 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "world.h"

#include <cstring> // memcpy

namespace vox {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

World::World(
	const PerlinNoise& noise,
	std::size_t width,
	std::size_t height
) {
	_origin.x = width / 2;
	_origin.z = height / 2;
	_origin.y = noise.noiseAt(_origin.x, _origin.z) + BLOCK_SIZE;
	_generateChunks(noise);
}

/* ========================================================================== */

/**
 * @brief Generates a vector that stores height data for each block in the world.
 * Will be stored in a constant buffer and used for instancing.
*/
std::vector<uint8_t>	World::generateHeightBuffer() const noexcept {
	std::vector<uint8_t>	height_buffer;

	height_buffer.reserve(_chunks.size() * CHUNK_AREA);

	std::size_t	offset = 0;
	for (const Chunk& chunk: _chunks) {
		const auto chunk_map = chunk.getHeightMap();
		memcpy(
			height_buffer.data() + offset,
			chunk_map.data(),
			chunk_map.size());
		offset += chunk_map.size() * sizeof(uint8_t);
	}

	return height_buffer;
}

/* ========================================================================== */

void	World::setOrigin(const scop::Vect3& origin) noexcept {
	_origin = origin;
}

const scop::Vect3&	World::getOrigin() const noexcept {
	return _origin;
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

void	World::_generateChunks(const PerlinNoise& noise) {
	// For now, only render distance is rendered
	constexpr std::size_t world_width = RENDER_DISTANCE;
	constexpr std::size_t world_depth = RENDER_DISTANCE;

	_chunks.reserve(world_width * world_depth);

	for (std::size_t z = 0; z < world_depth; ++z) {
		for (std::size_t x = 0; x < world_width; ++x) {
			_chunks.emplace_back(Chunk(noise, x, 0, z));
		}
	}
}

} // namespace vox