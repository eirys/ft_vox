/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:19:11 by etran             #+#    #+#             */
/*   Updated: 2023/06/04 21:40:23 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Std
# include <array> // std::array

# include "vector.h"

namespace vox {

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

	typedef std::array<scop::Vect3, 4>	Face;

	/* ========================================================================= */
	/*                               CONST MEMBERS                               */
	/* ========================================================================= */

	static constexpr float	size = 1.0f;

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

	/* FACES =================================================================== */

	void				top(Face& face) const noexcept;
	void				bottom(Face& face) const noexcept;
	void				left(Face& face) const noexcept;
	void				right(Face& face) const noexcept;
	void				front(Face& face) const noexcept;
	void				back(Face& face) const noexcept;

	Face				top_face() const noexcept;
	Face				bottom_face() const noexcept;
	Face				left_face() const noexcept;
	Face				right_face() const noexcept;
	Face				front_face() const noexcept;
	Face				back_face() const noexcept;

	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	scop::Vect3			pos;

}; // class Cube

} // namespace vox