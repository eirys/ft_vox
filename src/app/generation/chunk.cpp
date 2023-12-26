/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 11:46:11 by etran             #+#    #+#             */
/*   Updated: 2023/12/24 15:28:34 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chunk.h"
#include "perlin_noise.h"
#include "vertex.h"
#include "packed_cube.h"
#include "mesh.h"
#include "bounding_frustum.h"
#include "player.h"
#include "block.h"

#include <cassert> // assert

namespace vox {

using IntersectionType = Chunk::IntersectionType;
using BoundingFrustum = Chunk::BoundingFrustum;
using Vect3 = ::scop::Vect3;

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

Chunk::Chunk(const PerlinNoise& noise, uint8_t x, uint8_t y, uint8_t z):
	BoundingBox(
		CHUNK_SIZE * Vect3((float)x, (float)y, (float)z) + Vect3(CHUNK_SIZE / 2.0f),
		Vect3(CHUNK_SIZE / 2.0f)),
	_x(x),
	_y(y),
	_z(z)
{
	_generateChunk(noise);
}

/* GFX ====================================================================== */

//DPCT
std::array<uint8_t, CHUNK_AREA>	Chunk::generateHeightMap() const noexcept {
	std::array<uint8_t, CHUNK_AREA>	height_map{};

	for (uint8_t z = 0; z < CHUNK_SIZE; ++z) {
		for (uint8_t x = 0; x < CHUNK_SIZE; ++x) {

			for (uint8_t y = CHUNK_SIZE - 1; y >= 0; --y) {
				const Block& block = getBlock(x, y, z);
				if (block.getType() != MaterialType::AIR) {
					height_map[z * CHUNK_SIZE + x] = y;
					break;
				}
			}

		}
	}
	return height_map;
}

//DPCT
bool	Chunk::isVisible(const BoundingFrustum& frustum) const {
	return _checkIntersection(frustum) != IntersectionType::Outside;
}

/**
 * @brief Packs each block data into 2 bytes and stores them in the chunk data array.
*/
void	Chunk::fillChunkMap(std::array<uint16_t, CHUNK_VOLUME>& data) const {
	for (uint8_t z = 0; z < CHUNK_SIZE; ++z) {
		for (uint8_t x = 0; x < CHUNK_SIZE; ++x) {
			for (uint8_t y = 0; y < CHUNK_SIZE; ++y) {
				const Block& block = getBlock(x, y, z);
				const uint32_t x_offsetted = x + (y % 4) * CHUNK_SIZE;
				const uint32_t z_offsetted = z + (y / 4) * CHUNK_SIZE;
				data[z_offsetted * CHUNK_SIZE + x_offsetted] = block.computePackedData();
			}
		}
	}
}

/* GAMEPLAY ================================================================= */

void	Chunk::updateActivity(const Player& player) {
	// check if player in RD radius
	if (scop::norm(player.getPosition() - _center) < CHUNK_SIZE * 2.0f)
		_isActive = true;
	else
		_isActive = false;
}

/* GETTER =================================================================== */

/**
 * @brief Upper left hand coordinates.
*/
Vect3	Chunk::getCoordinates() const noexcept {
	return CHUNK_SIZE * Vect3(static_cast<float>(_x), static_cast<float>(_y), static_cast<float>(_z));
}

Vect3	Chunk::getCenterCoordinates() const noexcept {
	return BoundingBox::_center;
}

/**
 * @brief Return block's local to chunk coordinates.
*/
const Block&	Chunk::getBlock(uint8_t x, uint8_t y, uint8_t z) const {
	assert(x < CHUNK_SIZE && y < CHUNK_SIZE && z < CHUNK_SIZE);
	return _blocks[z * CHUNK_AREA + (y * CHUNK_SIZE + x)];
}

/**
 * @brief Return block's local to chunk coordinates.
*/
Block&	Chunk::getBlock(uint8_t x, uint8_t y, uint8_t z) {
	assert(x < CHUNK_SIZE && y < CHUNK_SIZE && z < CHUNK_SIZE);
	return _blocks[z * CHUNK_AREA + (y * CHUNK_SIZE + x)];
}

bool	Chunk::isActive() const noexcept {
	return _isActive;
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

IntersectionType	Chunk::_checkIntersection(const BoundingFrustum& frustum) const {
	bool isIntersecting = false;
	for (const Plane& plane: frustum.planes) {
		IntersectionType planeIntersection = _checkPlaneIntersection(plane);

		if (planeIntersection == IntersectionType::Outside)
			return IntersectionType::Outside;
		else if (planeIntersection == IntersectionType::Intersecting)
			isIntersecting = true;
	}

	if (isIntersecting)
		return IntersectionType::Intersecting;
	return IntersectionType::Inside;
}

/* ========================================================================== */

/**
 * @brief Fills chunk volume data with blocks.
*/
void	Chunk::_generateChunk(const PerlinNoise& perlin_noise) {
	_blocks.resize(CHUNK_VOLUME);

	for (uint8_t z = 0; z < CHUNK_SIZE; ++z) {
		for (uint8_t x = 0; x < CHUNK_SIZE; ++x) {
			float y = perlin_noise.noiseAt(_x * CHUNK_SIZE + x, _z * CHUNK_SIZE + z);
			Block& block = getBlock(x, static_cast<uint8_t>(y), z);
			block.setType(MaterialType::GRASS);
			_fillColumn(x, static_cast<uint8_t>(y), z);
		}
	}
}

void	Chunk::_fillColumn(uint8_t x, uint8_t max_height, uint8_t z) noexcept {
	for (uint8_t y = 0; y < max_height; ++y) {
		Block& block = getBlock(x, y, z);
		block.setType(MaterialType::DIRT);
	}
}

} // namespace vox
