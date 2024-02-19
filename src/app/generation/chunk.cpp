/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 11:46:11 by etran             #+#    #+#             */
/*   Updated: 2024/01/25 15:59:32 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chunk.h"
#include "perlin_noise.h"
#include "vertex.h"
#include "mesh.h"
#include "bounding_frustum.h"
#include "player.h"
#include "block.h"

#include <cassert> // assert
#include <algorithm> // std::clamp

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

static constexpr void	_getBlockOffset(BlockFace face, std::array<int8_t, 3>& offset) {
	switch (face) {
		case BlockFace::RIGHT:
			offset = { 1, 0, 0 };
			break;
		case BlockFace::LEFT:
			offset = { -1, 0, 0 };
			break;
		case BlockFace::TOP:
			offset = { 0, 1, 0 };
			break;
		case BlockFace::BOTTOM:
			offset = { 0, -1, 0 };
			break;
		case BlockFace::FRONT:
			offset = { 0, 0, 1 };
			break;
		case BlockFace::BACK:
			offset = { 0, 0, -1 };
			break;
		default:
			assert(false);
	}
}

/**
 * @brief Packs each block data into 2 bytes and stores them in the chunk data array.
*/
void	Chunk::fillChunkMap(std::array<uint32_t, CHUNK_VOLUME>& data) const {
	auto computeBlockPackedData = [this](uint8_t x, uint8_t y, uint8_t z) -> uint32_t {
		const Block& block = getBlock(x, y, z);
		uint32_t packed_data = block.computePackedData();
		if (!block.isEmpty()) {
			uint8_t visible_faces = 0;

			for (uint8_t face = 0; face < 6; ++face) {
				BlockFace block_face = static_cast<BlockFace>(face);

				std::array<int8_t, 3> offset;
				_getBlockOffset(block_face, offset);
				offset[0] += x;
				offset[1] += y;
				offset[2] += z;

				// TODO:
				// Check if block is in chunk. If not, check if block is in neighbor chunk.
				// For now, just consider the face visible
				if (offset[0] < 0 || offset[0] >= CHUNK_SIZE ||
					offset[1] < 0 || offset[1] >= CHUNK_SIZE ||
					offset[2] < 0 || offset[2] >= CHUNK_SIZE) {
					visible_faces |= (1 << face);
				} else {
					const Block& neighbor = getBlock(offset[0], offset[1], offset[2]);
					// If neighbor isn't opaque, consider face visible
					if (!neighbor.isOpaque())
						visible_faces |= (1 << face);
				}
			}
			// Format is
			// 0, 15: [packed_data] (16 bits)
			// 16, 21: [visible_faces] (6 bits)
			// 22, 31: [unused] (10 bits)

			packed_data |= (visible_faces << 16);
		}
		return packed_data;
	};

	// 16 blocks in x and z, y subdivided in rows of 4
	constexpr const uint32_t LINE_SIZE = CHUNK_SIZE * 4;

	for (uint8_t z = 0; z < CHUNK_SIZE; ++z) {
		for (uint8_t x = 0; x < CHUNK_SIZE; ++x) {
			for (uint8_t y = 0; y < CHUNK_SIZE; ++y) {
				const uint32_t x_offsetted = (x * 4) + (y % 4);
				const uint32_t z_offsetted = (z * 4) + (y / 4);

				data[z_offsetted * LINE_SIZE + x_offsetted] = computeBlockPackedData(x, y, z);
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
	static const constexpr uint32_t chunk_height = 256;

	_blocks.resize(CHUNK_SIZE * CHUNK_SIZE * chunk_height);

	for (uint8_t z = 0; z < CHUNK_SIZE; ++z) {
		for (uint8_t x = 0; x < CHUNK_SIZE; ++x) {
			float height = perlin_noise.noiseAt(_x * CHUNK_SIZE + x, _z * CHUNK_SIZE + z);
			uint8_t y = std::floor(height);
			Block& block = getBlock(x, y, z);

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
