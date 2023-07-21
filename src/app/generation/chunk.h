/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 12:07:38 by etran             #+#    #+#             */
/*   Updated: 2023/07/21 20:53:23 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Std
# include <cstdint>
# include <array> // std::array

# define BLOCK_SIZE			1
# define CHUNK_SIZE			16
# define CHUNK_HEIGHT		256
# define RENDER_DISTANCE	16

# define CHUNK_COUNT		1024
# define WORLD_WIDTH		CHUNK_COUNT * CHUNK_SIZE		// 16384
# define WORLD_DEPTH		WORLD_WIDTH						// 16384
# define WORLD_HEIGHT		CHUNK_HEIGHT					// 256

# define RENDER_WIDTH		RENDER_DISTANCE * CHUNK_SIZE	// 256
# define RENDER_DEPTH		RENDER_DISTANCE * CHUNK_SIZE	// 256

// # include "block.h"

namespace vox {

// /**
//  * @brief Holds data about a chunk (local cube positions, etc.).
// */
// class Chunk {
// public:
// 	/* ========================================================================= */
// 	/*                                  TYPEDEFS                                 */
// 	/* ========================================================================= */

// 	/**
// 	 * @brief A column of blocks with y coordinate ranging from 0 to 255.
// 	*/
// 	typedef	std::array<Block, CHUNK_HEIGHT>		ChunkColumn;

// 	/**
// 	 * @brief A row of block columns with x coordinate ranging from 0 to 15.
// 	*/
// 	typedef	std::array<ChunkColumn, CHUNK_SIZE>	ChunkRow;

// 	/* ========================================================================= */
// 	/*                                  METHODS                                  */
// 	/* ========================================================================= */

// 	Chunk(int8_t x, int8_t z);

// 	~Chunk() = default;

// 	Chunk() = delete;
// 	Chunk(Chunk&& src) = delete;
// 	Chunk(const Chunk& src) = delete;
// 	Chunk &operator=(Chunk&& rhs) = delete;
// 	Chunk &operator=(const Chunk& rhs) = delete;

// private:
// 	/* ========================================================================= */
// 	/*                               CLASS MEMBERS                               */
// 	/* ========================================================================= */

// 	int8_t								_x;
// 	int8_t								_z;
// 	std::array<ChunkRow, CHUNK_SIZE>	_blocks;

// }; // class Chunk

/**
 * @brief Converts a float to a 32-bit integer.
*/
inline int32_t convert(float x, uint8_t shift = 0) noexcept {
	return x * (1 << shift);
}

/**
 * @brief Converts a position to a 32-bit integer.
 *
 * @note x (4 bits) | y (8 bits) | z (4 bits) | chunk address (16 bits)
 * @note x, y and z are local to the chunk.
 *
 * @note The chunk address is composed of the x and z chunk coordinates:
 * @note x_chunk (8 bits) | z_chunk (8 bits)
*/
inline int32_t	toChunkPos(float x, float y, float z) noexcept {
	// TODO: Add render distance in ubo
	int16_t chunk_address =
		(int32_t)x / CHUNK_SIZE |
		(int32_t)(z / CHUNK_SIZE) << 8;

	x = (int32_t)x % CHUNK_SIZE;
	z = (int32_t)z % CHUNK_SIZE;

	return
		(int32_t)x |
		(int32_t)y << 4 |
		(int32_t)z << 12 |
		chunk_address << 16;
}

} // namespace vox