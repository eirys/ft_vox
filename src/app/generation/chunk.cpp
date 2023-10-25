/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 11:46:11 by etran             #+#    #+#             */
/*   Updated: 2023/10/25 17:35:00 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chunk.h"
#include "perlin_noise.h"
#include "vertex.h"
#include "packed_cube.h"
#include "mesh.h"
#include "bounding_frustum.h"
#include "player.h"

#include <cassert> // assert

namespace vox {

using IntersectionType = scop::graphics::IntersectionType;

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

Chunk::Chunk(const PerlinNoise& noise, uint8_t x, uint8_t y, uint8_t z):
	BoundingBox(
		Vect3{x + (CHUNK_SIZE / 2.0f), y + (CHUNK_SIZE / 2.0f), z + (CHUNK_SIZE / 2.0f)},
		Vect3{CHUNK_SIZE / 2.0f}
	), _x(x), _y(y), _z(z) {
		_generateChunk(noise);
}

/* ========================================================================== */

std::array<uint8_t, CHUNK_AREA>	Chunk::generateHeightMap() const noexcept {
	std::array<uint8_t, CHUNK_AREA>	height_map{};

	for (uint8_t z = 0; z < CHUNK_SIZE; ++z) {
		for (uint8_t x = 0; x < CHUNK_SIZE; ++x) {

			for (uint8_t y = CHUNK_SIZE - 1; y >= 0; --y) {
				const Block& block = getBlock(x, y, z);
				if (block.getType() != MaterialType::MATERIAL_AIR) {
					height_map[z * CHUNK_SIZE + x] = y;
					break;
				}
			}

		}
	}
	return height_map;
}

IntersectionType	Chunk::checkIntersection(const scop::graphics::BoundingFrustum& frustum) const {
	bool isIntersecting = false;
	for (const Plane& plane: frustum.planes) {
		IntersectionType planeIntersection = checkPlaneIntersection(plane);

		if (planeIntersection == IntersectionType::Outside)
			return IntersectionType::Outside;
		else if (planeIntersection == IntersectionType::Intersecting)
			isIntersecting = true;
	}

	if (isIntersecting)
		return IntersectionType::Intersecting;
	return IntersectionType::Inside;
}

void	Chunk::updateActivity(const Player& player) {
	// check if player in RD radius
	if (scop::norm(player.getPosition() - _center) < CHUNK_SIZE * 2.0f)
		_isActive = true;
	else
		_isActive = false;
}

/* GETTER =================================================================== */

uint32_t	Chunk::getChunkCoordinates() const noexcept {
	return (_x << 16) | (_y << 8) | _z;
}

Block&	Chunk::getBlock(uint8_t x, uint8_t y, uint8_t z) noexcept {
	assert(x < CHUNK_SIZE && y < CHUNK_SIZE && z < CHUNK_SIZE);
	return _blocks[z * CHUNK_AREA + (y * CHUNK_SIZE + x)];
}

const Block&	Chunk::getBlock(uint8_t x, uint8_t y, uint8_t z) const noexcept {
	assert(x < CHUNK_SIZE && y < CHUNK_SIZE && z < CHUNK_SIZE);
	return _blocks[z * CHUNK_AREA + (y * CHUNK_SIZE + x)];
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
			Block& block = getBlock(x, y, z);
			block.setType(MaterialType::MATERIAL_GRASS);
			_fillColumn(x, y, z);
		}
	}
}

void	Chunk::_fillColumn(std::size_t x, std::size_t max_height, std::size_t z) noexcept {
	for (uint8_t y = 0; y < max_height; ++y) {
		Block& block = getBlock(x, y, z);
		block.setType(MaterialType::MATERIAL_DIRT);
	}
}

} // namespace vox
