/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 12:07:38 by etran             #+#    #+#             */
/*   Updated: 2023/07/16 11:01:34 by etran            ###   ########.fr       */
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
s
namespace vox {

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
 * @note x_chunk (8 bits) | z_chunk (8 bits) | y_chunk (4 bits)
*/
inline int32_t	toChunkPos(float x, float y, float z) noexcept {
	// TODO: Add render distance in ubo
	int32_t chunk_address =
		(int32_t)x / CHUNK_SIZE |
		((int32_t)z / CHUNK_SIZE) << 8 |
		((int32_t)y / CHUNK_SIZE) << 12;

	x = (int32_t)x % CHUNK_SIZE;
	y = (int32_t)y % CHUNK_SIZE;
	z = (int32_t)z % CHUNK_SIZE;

	return
		static_cast<int32_t>(x) |
		static_cast<int32_t>(y) << 4 |
		static_cast<int32_t>(z) << 8 |
		chunk_address << 16;
}

} // namespace vox