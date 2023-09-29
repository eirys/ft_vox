/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 12:07:38 by etran             #+#    #+#             */
/*   Updated: 2023/09/25 14:40:12 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Std
# include <array> // std::vector
# include <vector> // std::vector

# include "block.h"
# include "chunk_macros.h"

namespace vox {

struct Vertex;
struct Mesh;
class PerlinNoise;

/**
 * @brief World subdivision. A chunk size is 16 x 16 x 16.
 *
 * @note x, y and z represent the chunk's position in the world.
*/
class Chunk final {
public:
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	using ChunkSlice = std::array<Block, CHUNK_AREA>; // slice y
	using ChunkRow = std::array<Block, CHUNK_SIZE>; // row x of slice y

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	Chunk(const PerlinNoise& noise, uint8_t x, uint8_t y, uint8_t z);

	Chunk() = default;
	Chunk(Chunk &&src) = default;
	Chunk &operator=(Chunk &&rhs) = default;
	~Chunk() = default;

	Chunk(const Chunk &src) = delete;
	Chunk &operator=(const Chunk &rhs) = delete;

	/* ========================================================================= */

	static Mesh						generateChunkMesh() noexcept;

	/* ========================================================================= */

	const std::array<uint8_t, CHUNK_AREA>&	getHeightMap() const noexcept;
	// std::array<uint8_t, CHUNK_AREA>	getHeightMap() const noexcept;
	uint32_t						getChunkCoordinates() const noexcept;

	const Block&					getBlock(
		uint8_t x,
		uint8_t y,
		uint8_t z) const noexcept;
	ChunkRow						getRow(uint8_t x, uint8_t y) const;
	ChunkSlice						getSlice(uint8_t y) const;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	uint8_t							_x;
	uint8_t							_y;
	uint8_t							_z;
	std::array<uint8_t, CHUNK_AREA>	_blocks{};
	// std::vector<Block>				_blocks;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void		_generateChunk(const PerlinNoise& noise);
	void		_fillColumn(
		std::size_t x,
		std::size_t y,
		MaterialType material = MaterialType::MATERIAL_DIRT);

}; // class Chunk

/**
 * @brief Converts a position to a 32-bit integer.
 *
 * @note x (4 bits) | y (4 bits) | z (4 bits) | chunk address (20 bits)
 * @note x, y and z are local to the chunk.
 *
 * @note The chunk address is composed of the x, y and z chunk coordinates:
 * @note x_chunk (8 bits) | y_chunk (4 bits) | z_chunk (8 bits)
*/
inline int32_t	toChunkPos(float x, float y, float z) noexcept {
	// xxxxxxxx yyyy zzzzzzzz
	int32_t x_chunk = static_cast<int32_t>(x) / CHUNK_SIZE;
	int32_t y_chunk = (static_cast<int32_t>(y) / CHUNK_SIZE) << 8;
	int32_t z_chunk = (static_cast<int32_t>(z) / CHUNK_SIZE) << 12;
	int32_t chunk_address = x_chunk | y_chunk | z_chunk;

	// xxxx yyyy zzzz
	x = static_cast<int32_t>(x) % CHUNK_SIZE;
	y = static_cast<int32_t>(y) % CHUNK_SIZE;
	z = static_cast<int32_t>(z) % CHUNK_SIZE;

	return
		static_cast<int32_t>(x) |
		static_cast<int32_t>(y) << 4 |
		static_cast<int32_t>(z) << 8 |
		chunk_address << 12;
}

} // namespace vox