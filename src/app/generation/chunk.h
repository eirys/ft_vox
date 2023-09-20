/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 12:07:38 by etran             #+#    #+#             */
/*   Updated: 2023/09/18 17:33:44 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Std
# include <array> // std::array

# include "block.h"

# define CHUNK_SIZE			16 // Number of blocks per chunk row
# define RENDER_DISTANCE	16 // Number of chunks to render

# define RENDER_WIDTH		RENDER_DISTANCE * CHUNK_SIZE			// 256
# define RENDER_DEPTH		RENDER_DISTANCE * CHUNK_SIZE			// 256

# define CHUNK_AREA			CHUNK_SIZE * CHUNK_SIZE					// 256
# define CHUNK_VOLUME		CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE	// 65536

// Number of vertices in a flat chunk mesh
# define BLOCK_VERTICES_COUNT		8
# define CHUNK_VERTICES_COUNT		BLOCK_VERTICES_COUNT * CHUNK_AREA

// Number of indices in a flat chunk mesh
# define TRIANGLE_VERTICES_COUNT	3
# define QUAD_VERTICES_COUNT		2 * TRIANGLE_VERTICES_COUNT
# define BLOCK_FACES_COUNT			6
# define CHUNK_INDICES_COUNT		CHUNK_AREA * BLOCK_FACES_COUNT * QUAD_VERTICES_COUNT

// namespace scop {

// struct Vertex;

// }
struct Vertex;

namespace vox {

class PerlinNoise;

/**
 * @brief World subdivision. A chunk size is 16 x 16 x 16.
*/
class Chunk final {
public:
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	using VerticeArray = std::array<Vertex, CHUNK_VERTICES_COUNT>;
	using IndexArray = std::array<uint32_t, CHUNK_INDICES_COUNT>;

	struct ChunkMesh {
		VerticeArray	vertices;
		IndexArray		indices;
	};

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

	static ChunkMesh	generateChunkMesh() noexcept;

	/* ========================================================================= */

	const Block&		getBlock(uint8_t x, uint8_t y, uint8_t z) const noexcept;
	const Block&		getBlock(uint32_t packed_coordinates) const noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	uint8_t							_x;
	uint8_t							_y;
	uint8_t							_z;
	std::array<Block, CHUNK_VOLUME>	_blocks{};

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