/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:19:11 by etran             #+#    #+#             */
/*   Updated: 2023/06/07 15:58:51 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Std
# include <array> // std::array

# include "vector.h"

namespace vox {

// TODO
enum MaterialType {
	GRASS,
	DIRT,
	STONE,
	WOOD,
	LEAVES,
	SAND,
	WATER,
	LAVA,
	ICE,
	SNOW
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

		const scop::Vect3&	operator[](std::size_t index) const noexcept;
		scop::Vect3&		operator[](std::size_t index) noexcept;
		scop::Vect3			normal() const noexcept;

	};
	
	/* ========================================================================= */
	/*                               CONST MEMBERS                               */
	/* ========================================================================= */

	static constexpr const float	size = 1.0f;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	Cube() = default;
	Cube(const Cube&) = default;
	Cube(Cube&&) = default;
	Cube& operator=(const Cube&) = default;
	Cube& operator=(Cube&&) = default;
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
	// MaterialType			material;

}; // class Cube

} // namespace vox