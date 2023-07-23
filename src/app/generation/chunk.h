/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 12:07:38 by etran             #+#    #+#             */
/*   Updated: 2023/07/23 14:03:36 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Std
# include <cstdint>
# include <array> // std::array

/**
 * @brief Max values for a block's x and z coordinates (local to chunk).
*/
# define CHUNK_SIZE			16

/**
 * @brief Max value for a block's y coordinate.
*/
# define CHUNK_HEIGHT		256

/**
 * @brief Max values for a chunk's x and z coordinates (world).
*/
# define CHUNK_COUNT		1024

/**
 * @brief Number of chunks rendered around the player.
*/
# define RENDER_DISTANCE	16
# define BLOCK_SIZE			1

/**
 * @brief Max values for a block's x and z coordinates (in world).
*/
# define WORLD_SIZE			CHUNK_COUNT * CHUNK_SIZE		// 16384
# define WORLD_HEIGHT		CHUNK_HEIGHT					// 256

# define RENDER_WIDTH		RENDER_DISTANCE * CHUNK_SIZE	// 256
# define RENDER_DEPTH		RENDER_DISTANCE * CHUNK_SIZE	// 256

# include "block.h"

namespace vox {

/**
 * @brief Holds data about a chunk (local cube positions, etc.).
 *
 * @note A chunk is a 16x256x16 cube of blocks.
*/
class Chunk {
public:
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	/**
	 * @brief A column of blocks with y coordinate ranging from 0 to 255.
	*/
	typedef	std::array<Block, CHUNK_HEIGHT>		ChunkColumn;

	/**
	 * @brief A row of block columns with x coordinate ranging from 0 to 15.
	*/
	typedef	std::array<ChunkColumn, CHUNK_SIZE>	ChunkRow;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	Chunk(int16_t x, int16_t z);

	Chunk(Chunk&& src) = default;
	Chunk &operator=(Chunk&& rhs) = default;
	~Chunk() = default;

	Chunk() = delete;
	Chunk(const Chunk& src) = delete;
	Chunk &operator=(const Chunk& rhs) = delete;

	/* ========================================================================= */

	void								addBlock(Block block);

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	int16_t								_x;	// min: 0, max: 1023
	int16_t								_z;	// min: 0, max: 1023
	std::array<ChunkRow, CHUNK_SIZE>	_blocks;

}; // class Chunk

/**
 * @brief Converts a float to a 32-bit integer.
*/
inline int32_t convert(float x, uint8_t shift = 0) noexcept {
	return x * (1 << shift);
}

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
	// TODO: Add render distance in ubo

	// TODO: uint?
	int32_t chunk_address =
		(int32_t)x / CHUNK_SIZE |
		(int32_t)y / CHUNK_SIZE << 8 |
		(int32_t)z / CHUNK_SIZE << 12;

	x = (int32_t)x % CHUNK_SIZE;
	y = (int32_t)y % CHUNK_SIZE;
	z = (int32_t)z % CHUNK_SIZE;

	return
		(int32_t)x |
		(int32_t)y << 4 |
		(int32_t)z << 8 |
		chunk_address << 12;
}

} // namespace vox