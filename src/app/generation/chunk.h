/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 12:07:38 by etran             #+#    #+#             */
/*   Updated: 2023/12/08 16:25:19 by etran            ###   ########.fr       */
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
class Chunk final: public scop::gfx::BoundingBox {
public:
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	using BoundingFrustum	= scop::gfx::BoundingFrustum;
	using IntersectionType	= scop::gfx::IntersectionType;
	using BoundingBox		= scop::gfx::BoundingBox;
	using Plane				= BoundingBox::Plane;

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

	/* GFX ===================================================================== */

	std::array<uint8_t, CHUNK_AREA>		generateHeightMap() const noexcept;
	bool								isVisible(const BoundingFrustum& frustum) const;

	/* GAMEPLAY ================================================================ */

	void			updateActivity(const Player& player);

	/* GETTERS ================================================================= */

	scop::Vect3		getCoordinates() const noexcept;
	scop::Vect3		getCenterCoordinates() const noexcept;
	const Block&	getBlock(uint8_t x, uint8_t y, uint8_t z) const;
	Block&			getBlock(uint8_t x, uint8_t y, uint8_t z);
	bool			isActive() const noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	std::vector<Block>		_blocks;
	bool					_isActive = false;

	// World index
	uint8_t					_x;
	uint8_t					_y;
	uint8_t					_z;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	IntersectionType	_checkIntersection(const BoundingFrustum& frustum) const override;

	/* ========================================================================= */

	void				_generateChunk(const PerlinNoise& noise);
	void				_fillColumn(uint8_t x, uint8_t max_height, uint8_t z) noexcept;

}; // class Chunk

} // namespace vox