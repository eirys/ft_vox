/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:50:12 by etran             #+#    #+#             */
/*   Updated: 2023/06/04 21:52:00 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

namespace vox {

typedef scop::Vect3	Vect3;

/**
 * Cube model:
 *   h_____g    Frame of reference:
 * e/|___f/|    y z
 * | d---|-c    |/
 * |/    |/     O___x
 * a_____b
*/

/* VERTICES ================================================================= */

// Vertice a
Vect3	Cube::lowerBottomLeft() const noexcept {
	return pos;
}

// Vertice b
Vect3	Cube::lowerBottomRight() const noexcept {
	return {pos.x + size, pos.y, pos.z};
}

// Vertice c
Vect3	Cube::lowerTopRight() const noexcept {
	return {pos.x + size, pos.y, pos.z + size};
}

// Vertice d
Vect3	Cube::lowerTopLeft() const noexcept {
	return {pos.x, pos.y, pos.z + size};
}

// Vertice e
Vect3	Cube::upperBottomLeft() const noexcept {
	return {pos.x, pos.y + size, pos.z + size};
}

// Vertice f
Vect3	Cube::upperBottomRight() const noexcept {
	return {pos.x + size, pos.y + size, pos.z + size};
}

// Vertice g
Vect3	Cube::upperTopRight() const noexcept {
	return {pos.x + size, pos.y + size, pos.z};
}

// Vertice h
Vect3	Cube::upperTopLeft() const noexcept {
	return {pos.x, pos.y + size, pos.z};
}

/* FACES ==================================================================== */

/**
 * Note: These functions populate the Cube::Face array.
*/

// Face e-f-g-h
void	Cube::top(Cube::Face& face) const noexcept {
	face[0] = upperBottomLeft();
	face[1] = upperBottomRight();
	face[2] = upperTopRight();
	face[3] = upperTopLeft();
}

// Face a-b-c-d
void	Cube::bottom(Cube::Face& face) const noexcept {
	face[0] = lowerBottomLeft();
	face[1] = lowerBottomRight();
	face[2] = lowerTopRight();
	face[3] = lowerTopLeft();
}

// Face a-d-h-e
void	Cube::left(Cube::Face& face) const noexcept {
	face[0] = lowerBottomLeft();
	face[1] = lowerTopLeft();
	face[2] = upperTopLeft();
	face[3] = upperBottomLeft();
}

// Face b-c-g-f
void	Cube::right(Cube::Face& face) const noexcept {
	face[0] = lowerBottomRight();
	face[1] = lowerTopRight();
	face[2] = upperTopRight();
	face[3] = upperBottomRight();
}

// Face a-b-f-e
void	Cube::back(Cube::Face& face) const noexcept {
	face[0] = lowerBottomLeft();
	face[1] = lowerBottomRight();
	face[2] = upperBottomRight();
	face[3] = upperBottomLeft();
}

// Face c-d-h-g
void	Cube::front(Cube::Face& face) const noexcept {
	face[0] = lowerTopRight();
	face[1] = lowerTopLeft();
	face[2] = upperTopLeft();
	face[3] = upperTopRight();
}

/* ========================================================================== */

// Face e-f-g-h
Cube::Face	Cube::top_face() const noexcept {
	return {
		upperBottomLeft(),
		upperBottomRight(),
		upperTopRight(),
		upperTopLeft()
	};
}

// Face a-b-c-d
Cube::Face	Cube::bottom_face() const noexcept {
	return {
		lowerBottomLeft(),
		lowerBottomRight(),
		lowerTopRight(),
		lowerTopLeft()
	};
}

// Face a-d-h-e
Cube::Face	Cube::left_face() const noexcept {
	return {
		lowerBottomLeft(),
		lowerTopLeft(),
		upperTopLeft(),
		upperBottomLeft()
	};
}

// Face b-c-g-f
Cube::Face	Cube::right_face() const noexcept {
	return {
		lowerBottomRight(),
		lowerTopRight(),
		upperTopRight(),
		upperBottomRight()
	};
}

// Face a-b-f-e
Cube::Face	Cube::back_face() const noexcept {
	return {
		lowerBottomLeft(),
		lowerBottomRight(),
		upperBottomRight(),
		upperBottomLeft()
	};
}

// Face c-d-h-g
Cube::Face	Cube::front_face() const noexcept {
	return {
		lowerTopRight(),
		lowerTopLeft(),
		upperTopLeft(),
		upperTopRight()
	};
}

} // namespace vox