/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 12:07:38 by etran             #+#    #+#             */
/*   Updated: 2023/10/06 12:52:32 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Std
# include <array> // std::vector
# include <vector> // std::vector

# include "block.h"
# include "chunk_macros.h"
# include "bounding_box.h"

namespace vox {

struct Vertex;
struct Mesh;
class PerlinNoise;
class Player;

/**
 * @brief World subdivision. A chunk size is 16 x 16 x 16.
 *
 * @note x, y and z represent the chunk's position in the world.
*/
class Chunk final: public scop::graphics::BoundingBox {
public:
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	using super = scop::graphics::BoundingBox;
	using Plane = super::Plane;

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

	std::array<uint8_t, CHUNK_AREA>		generateHeightMap() const noexcept;
	// Rename to isVisible* ?
	scop::graphics::IntersectionType	checkIntersection(
		const scop::graphics::BoundingFrustum& frustum) const override;

	void								updateActivity(const Player& player);

	/* GETTERS ================================================================= */
	uint32_t		getChunkCoordinates() const noexcept;
	const Block&	getBlock(uint8_t x, uint8_t y, uint8_t z) const noexcept;
	Block&			getBlock(uint8_t x, uint8_t y, uint8_t z)  noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	uint8_t							_x;
	uint8_t							_y;
	uint8_t							_z;
	std::vector<Block>				_blocks;

	bool							_isActive = false;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void		_generateChunk(const PerlinNoise& noise);
	void		_fillColumn(std::size_t x, std::size_t max_height, std::size_t z) noexcept;

}; // class Chunk

} // namespace vox