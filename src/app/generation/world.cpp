/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 17:42:36 by etran             #+#    #+#             */
/*   Updated: 2023/11/16 23:26:44 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "world.h"
#include "perlin_noise.h"
#include "utils.h"
#include "block_properties.h"

#include <cstring> // memcpy
#include <cassert> // assert

namespace vox {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

World::World(
	const PerlinNoise& noise,
	std::size_t width,
	std::size_t height
) {
	LOG("Generating world...");
	_origin.x = static_cast<float>(width) / 2;
	_origin.z = static_cast<float>(height) / 2;

	float noise_value = noise.noiseAt(
		static_cast<std::size_t>(_origin.x),
		static_cast<std::size_t>(_origin.z));
	_origin.y = noise_value + BLOCK_SIZE;

	_generateChunks(noise);
	LOG("World generated.");
}

World::World(World&& other) {
	*this = std::move(other);
}

World& World::operator=(World&& other) {
	if (this == &other)
		return *this;
	_origin = std::move(other._origin);
	_chunks = std::move(other._chunks);
	return *this;
}

/* ========================================================================== */

// DEPRECATED
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

std::vector<uint16_t>	World::generateTerrainData() const {
	std::vector<uint16_t> 				world_data;

	world_data.reserve(_chunks.size() * CHUNK_VOLUME);

	std::array<uint16_t, CHUNK_VOLUME>	chunk_data;
	uint32_t							offset = 0;
	for (const Chunk& chunk: _chunks) {
		chunk.fillChunkMap(chunk_data);
		memcpy(
			world_data.data() + offset,
			chunk_data.data(),
			chunk_data.size());
		offset += chunk_data.size() * sizeof(uint16_t);
	}
}


/* SETTER/GETTER ============================================================ */

void	World::setOrigin(const scop::Vect3& origin) noexcept {
	_origin = origin;
}

const scop::Vect3&	World::getOrigin() const noexcept {
	return _origin;
}

/* ========================================================================== */

const std::vector<Chunk>&	World::getChunks() const noexcept {
	return _chunks;
}

/* ========================================================================== */

const Chunk&	World::getChunk(uint8_t x, uint8_t y, uint8_t z) const {
	//TODO: replace by y
	(void)y;
	return _chunks[z * _render_distance + (0 * _render_distance + x)];
}

Chunk&	World::getChunk(uint8_t x, uint8_t y, uint8_t z) {
	//TODO
	(void)y;
	return _chunks[z * _render_distance + (0 * _render_distance + x)];
}

/* ========================================================================== */

static std::array<uint8_t, 3>	_extractChunkIndex(float x, float y, float z) {
	std::array<uint8_t, 3>	chunk_index;

	chunk_index[0] = (int)x / CHUNK_SIZE;
	chunk_index[1] = (int)y / CHUNK_SIZE;
	chunk_index[2] = (int)z / CHUNK_SIZE;

	return chunk_index;
}

static std::array<uint8_t, 3>	_extractBlockIndex(float x, float y, float z) {
	std::array<uint8_t, 3>	block_pos;

	block_pos[0] = (int)x % CHUNK_SIZE;
	block_pos[1] = (int)y % CHUNK_SIZE;
	block_pos[2] = (int)z % CHUNK_SIZE;

	return block_pos;
}

/**
 * @brief Get block at world position.
 * Note: prefer using getChunk(index[3]).getBlock(x,y,z).
*/
const Block&	World::getBlock(float x, float y, float z) const {
	assert(x < _world_width && z < _world_depth && y < WORLD_HEIGHT);

	auto chunk_index = _extractChunkIndex(x, y, z);
	const Chunk& chunk = getChunk(chunk_index[0], chunk_index[1], chunk_index[2]);

	auto block_pos = _extractBlockIndex(x, y, z);
	return chunk.getBlock(block_pos[0], block_pos[1], block_pos[2]);
}

/**
 * @brief Get block at world position.
 * Note: prefer using getChunk(index[3]).getBlock(x,y,z).
*/
Block&	World::getBlock(float x, float y, float z) {
	assert(x < _world_width && z < _world_depth && y < WORLD_HEIGHT);

	auto chunk_index = _extractChunkIndex(x, y, z);
	Chunk& chunk = getChunk(chunk_index[0], chunk_index[1], chunk_index[2]);

	auto block_pos = _extractBlockIndex(x, y, z);
	return chunk.getBlock(block_pos[0], block_pos[1], block_pos[2]);
}

/**
 * @brief Get block at world position.
 * Note: prefer using getChunk(index[3]).getBlock(x,y,z).
*/
const Block&	World::getBlock(const scop::Vect3& pos) const {
	return getBlock(pos.x, pos.y, pos.z);
}

/**
 * @brief Get block at world position.
 * Note: prefer using getChunk(index[3]).getBlock(x,y,z).
*/
Block&	World::getBlock(const scop::Vect3& pos) {
	return getBlock(pos.x, pos.y, pos.z);
}

/* ========================================================================== */

float	World::getWidth() const noexcept {
	return _world_width;
}

float	World::getDepth() const noexcept {
	return _world_depth;
}

/**
 * @brief Returns number of chunks.
*/
uint8_t	World::getRenderDistance() const noexcept {
	return _render_distance;
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

void	World::_generateChunks(const PerlinNoise& noise) {
	_chunks.reserve(_render_distance * _render_distance);

	for (uint32_t z = 0; z < _render_distance; ++z) {
		for (uint32_t x = 0; x < _render_distance; ++x) {
			_chunks.emplace_back(Chunk(noise, x, 0, z));
		}
	}
}

} // namespace vox