/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:19:11 by etran             #+#    #+#             */
/*   Updated: 2023/09/18 17:38:34 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "vector.h"

namespace vox {

// Total material types possible: 256
enum class MaterialType: uint8_t {
	MATERIAL_AIR		= 0,

	MATERIAL_GRASS		= 1,
	MATERIAL_DIRT		= 2,
	MATERIAL_STONE		= 3,
	MATERIAL_WOOD		= 4,
	MATERIAL_LEAVES		= 5,
	MATERIAL_SAND		= 6,
	MATERIAL_WATER		= 7,
	MATERIAL_LAVA		= 8,
	MATERIAL_ICE		= 9,
	MATERIAL_SNOW		= 10
};

enum class FaceType: uint8_t {
	FACE_LEFT,
	FACE_RIGHT,
	FACE_FRONT,
	FACE_BACK,
	FACE_TOP,
	FACE_BOTTOM
};

/**
 * @brief Cube handler. Generates a cube from a position.
 *
 * @note Its position is represented by the bottom left corner of its lower face.
 *
 *   h_____g    Frame of reference:
 * e/|___f/|    y z
 * | d---|-c    |/
 * |/    |/     O___x
 * a_____b
*/
struct Cube {
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	/**
	 * @brief Face handler. Contains the vertices of a face, counter clockwise order.
	*/
	struct Face {
		scop::Vect3	vertices[4];
		FaceType	side;

		scop::Vect3	normal() const noexcept;
	};

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	Cube(const scop::Vect3& pos);

	Cube() = default;
	Cube(Cube&& x) = default;
	Cube(const Cube& x) = default;
	Cube& operator=(Cube&& x) = default;
	Cube& operator=(const Cube& x) = default;
	~Cube() = default;

	/* VERTICES ================================================================ */

	scop::Vect3			lowerBottomLeft() const noexcept;
	scop::Vect3			lowerBottomRight() const noexcept;
	scop::Vect3			lowerTopLeft() const noexcept;
	scop::Vect3			lowerTopRight() const noexcept;

	scop::Vect3			upperBottomLeft() const noexcept;
	scop::Vect3			upperBottomRight() const noexcept;
	scop::Vect3			upperTopLeft() const noexcept;
	scop::Vect3			upperTopRight() const noexcept;

	scop::Vect3			center() const noexcept;

	/* FACES =================================================================== */

	Face				top() const noexcept;
	Face				bottom() const noexcept;
	Face				left() const noexcept;
	Face				right() const noexcept;
	Face				front() const noexcept;
	Face				back() const noexcept;

	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	scop::Vect3			pos;
	MaterialType		material = MaterialType::MATERIAL_GRASS;

}; // class Cube

} // namespace vox