/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:50:12 by etran             #+#    #+#             */
/*   Updated: 2023/06/04 22:39:48 by etran            ###   ########.fr       */
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
	return {pos.x, pos.y + size, pos.z};
}

// Vertice f
Vect3	Cube::upperBottomRight() const noexcept {
	return {pos.x + size, pos.y + size, pos.z};
}

// Vertice g
Vect3	Cube::upperTopRight() const noexcept {
	return {pos.x + size, pos.y + size, pos.z + size};
}

// Vertice h
Vect3	Cube::upperTopLeft() const noexcept {
	return {pos.x, pos.y + size, pos.z + size};
}

/* FACES ==================================================================== */

// Face e-f-g-h
Cube::Face	Cube::top() const noexcept {
	return {{
		upperBottomLeft(),
		upperBottomRight(),
		upperTopRight(),
		upperTopLeft()
	}};
}

// Face a-b-c-d
Cube::Face	Cube::bottom() const noexcept {
	return {{
		lowerBottomLeft(),
		lowerBottomRight(),
		lowerTopRight(),
		lowerTopLeft()
	}};
}

// Face a-d-h-e
Cube::Face	Cube::left() const noexcept {
	return {{
		lowerBottomLeft(),
		lowerTopLeft(),
		upperTopLeft(),
		upperBottomLeft()
	}};
}

// Face b-c-g-f
Cube::Face	Cube::right() const noexcept {
	return {{
		lowerBottomRight(),
		lowerTopRight(),
		upperTopRight(),
		upperBottomRight()
	}};
}

// Face a-b-f-e
Cube::Face	Cube::back() const noexcept {
	return {{
		lowerBottomLeft(),
		lowerBottomRight(),
		upperBottomRight(),
		upperBottomLeft()
	}};
}

// Face c-d-h-g
Cube::Face	Cube::front() const noexcept {
	return {{
		lowerTopRight(),
		lowerTopLeft(),
		upperTopLeft(),
		upperTopRight()
	}};
}

} // namespace vox