/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 12:07:38 by etran             #+#    #+#             */
/*   Updated: 2023/07/07 16:56:30 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Std
# include <cstddef>

# define BLOCK_SIZE			1
# define CHUNK_SIZE			16
# define CHUNK_HEIGHT		256
# define RENDER_DISTANCE	16

# define WORLD_WIDTH		1024 * CHUNK_SIZE				// 16384
# define WORLD_DEPTH		WORLD_WIDTH						// 16384
# define WORLD_HEIGHT		CHUNK_HEIGHT					// 256

# define RENDER_WIDTH		RENDER_DISTANCE * CHUNK_SIZE	// 256
# define RENDER_DEPTH		RENDER_DISTANCE * CHUNK_SIZE	// 256

namespace vox {

/**
 * @brief Converts a float to a 8 bit integer.
*/
int32_t convert(float x, uint8_t shift = 0) noexcept {
	return x * (1 << shift);
}

/**
 * @brief Converts a position to a 32 bit integer.
 * @note The positions are stored in a chunk format:
 * x, y and z are local to the chunk.
 * The chunk address is stored in the 4th byte.
*/
int32_t	toChunkPos(float x, float y, float z) noexcept {
	// Offset to avoid negative values
	x += RENDER_WIDTH;
	z += RENDER_DEPTH;

	int16_t chunk_address =
		(int)x / CHUNK_SIZE |
		((int)z / CHUNK_SIZE) << 8;

	x = (int)x % CHUNK_SIZE;
	z = (int)z % CHUNK_SIZE;

	return
		convert(x) |
		convert(y, 8) |
		convert(z, 16) |
		convert(chunk_address, 24);
}

} // namespace vox