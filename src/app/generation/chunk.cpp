/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 11:46:11 by etran             #+#    #+#             */
/*   Updated: 2023/10/06 13:04:34 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chunk.h"
#include "perlin_noise.h"
#include "vertex.h"
#include "packed_cube.h"
#include "mesh.h"

namespace vox {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

Chunk::Chunk(const PerlinNoise& noise, uint8_t x, uint8_t y, uint8_t z):
	_x(x), _y(y), _z(z) {
		_generateChunk(noise);
}

/* ========================================================================== */

std::array<uint8_t, CHUNK_AREA>	Chunk::getHeightMap() const noexcept {
	std::array<uint8_t, CHUNK_AREA>	height_map{};

	for (uint8_t z = 0; z < CHUNK_SIZE; ++z) {
		for (uint8_t x = 0; x < CHUNK_SIZE; ++x) {

			for (uint8_t y = 0; y < CHUNK_SIZE; ++y) {
				const Block& block = _blocks[z * CHUNK_AREA + (y * CHUNK_SIZE + x)];
				if (block.getType() != MaterialType::MATERIAL_AIR) {
					height_map[z * CHUNK_SIZE + x] = y;
					break;
				}
			}

		}
	}
	return height_map;
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

/**
 * @brief Fills chunk volume data with blocks.
*/
void	Chunk::_generateChunk(const PerlinNoise& perlin_noise) {
	_blocks.resize(CHUNK_VOLUME);

	for (uint8_t z = 0; z < CHUNK_SIZE; ++z) {
		for (uint8_t x = 0; x < CHUNK_SIZE; ++x) {
			float y = perlin_noise.noiseAt(_x * CHUNK_SIZE + x, _z * CHUNK_SIZE + z);
			Block& block = _blocks[z * CHUNK_AREA + (y * CHUNK_SIZE + x)];
			block.setType(MaterialType::MATERIAL_GRASS);
			// _fillColumn(x, y);
		}
	}
}

} // namespace vox
