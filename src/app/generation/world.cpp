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
#include "perlin_noise.h"

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
		const auto chunk_map = chunk.generateHeightMap();
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

const std::vector<Chunk>&	World::getChunks() const noexcept {
	return _chunks;
}

const Chunk&	World::getChunk(uint8_t x, uint8_t y, uint8_t z) const noexcept {
	//TODO: replace by y
	return _chunks[z * _world_depth + (0 * _world_width + x)];
}

Chunk&	World::getChunk(uint8_t x, uint8_t y, uint8_t z) noexcept {
	//TODO
	return _chunks[z * _world_depth + (0 * _world_width + x)];
}

uint32_t	World::getWidth() const noexcept {
	return _world_width;
}

uint32_t	World::getDepth() const noexcept {
	return _world_depth;
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

void	World::_generateChunks(const PerlinNoise& noise) {
	_chunks.reserve(_world_width * _world_depth);

	for (uint32_t z = 0; z < _world_depth; ++z) {
		for (uint32_t x = 0; x < _world_width; ++x) {
			_chunks.emplace_back(Chunk(noise, x, 0, z));
		}
	}
}

} // namespace vox