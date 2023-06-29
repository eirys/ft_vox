/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:50:12 by etran             #+#    #+#             */
/*   Updated: 2023/06/28 18:32:05 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

namespace vox {

/**
 * Cube model:
 *   h_____g    Frame of reference:
 * e/|___f/|    y z
 * | d---|-c    |/
 * |/    |/     O___x
 * a_____b
*/

typedef scop::Vect2	Vect2;
typedef scop::Vect3	Vect3;

/* ========================================================================== */
/* ========================================================================== */

/*                                 STRUCT FACE                                */

/* ========================================================================== */
/* ========================================================================== */

scop::Vect3	Cube::Face::normal() const noexcept {
	switch (side) {
		case FACE_TOP:
			return {0.0f, 1.0f, 0.0f};	// +y
		case FACE_BOTTOM:
			return {0.0f, -1.0f, 0.0f};	// -y
		case FACE_RIGHT:
			return {1.0f, 0.0f, 0.0f};	// +x
		case FACE_LEFT:
			return {-1.0f, 0.0f, 0.0f};	// -x
		case FACE_FRONT:
			return {0.0f, 0.0f, 1.0f};	// +z
		default:
			return {0.0f, 0.0f, -1.0f};	// -z
	}
}

/* ========================================================================== */
/* ========================================================================== */

/*                                 CLASS CUBE                                 */

/* ========================================================================== */
/* ========================================================================== */

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

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

// Center of upper face
Vect3	Cube::center() const noexcept {
	return {
		std::fma(size, 0.5f, pos.x),
		pos.y + size,
		std::fma(size, 0.5f, pos.z)
	};
}

/* FACES ==================================================================== */

/**
 * @details Here is the mapping of the texture coordinates:
 * 
 * x: 0    0.25  0.5   0.75   1
 *                               y:
 *                h ___ g        0
 *                |  T  |
 *    g ___ h ___ e ___ f ___ g  0.25
 *    |  N  |  W  |  S  |  E  |
 *    c ___ d ___ a ___ b ___ c  0.5
 *                |  B  |
 *                d ___ c        0.75
 * 
 * { Empty line }                1
*/

// Face e-f-g-h
Cube::Face	Cube::top() const noexcept {
	return {{
		upperBottomLeft(),
		upperBottomRight(),
		upperTopRight(),
		upperTopLeft()
	}, FaceType::FACE_TOP
	// }, {
	// 	{0.5f, 0.25f},
	// 	{0.75f, 0.25f},
	// 	{0.75f, 0.0f},
	// 	{0.5f, 0.0f}
	};
}

// Face a-b-c-d
Cube::Face	Cube::bottom() const noexcept {
	return {{
		lowerBottomLeft(),
		lowerBottomRight(),
		lowerTopRight(),
		lowerTopLeft()
	}, FaceType::FACE_BOTTOM
	// }, {
	// 	{0.5f, 0.5f},
	// 	{0.75f, 0.5f},
	// 	{0.75f, 0.75f},
	// 	{0.5f, 0.75f}
	};
}

// Face d-a-e-h
Cube::Face	Cube::left() const noexcept {
	return {{
		lowerTopLeft(),
		lowerBottomLeft(),
		upperBottomLeft(),
		upperTopLeft()
	}, FaceType::FACE_LEFT
	};
	// }, {
	// 	{0.25f, 0.5f},
	// 	{0.5f, 0.5f},
	// 	{0.5f, 0.25f},
	// 	{0.25f, 0.25f}
	// }};
}

// Face b-c-g-f
Cube::Face	Cube::right() const noexcept {
	return {{
		lowerBottomRight(),
		lowerTopRight(),
		upperTopRight(),
		upperBottomRight()
	}, FaceType::FACE_RIGHT
	};
	// }, {
	// 	{0.75f, 0.5f},
	// 	{1.0f, 0.5f},
	// 	{1.0f, 0.25f},
	// 	{0.75f, 0.25f}
	// }};
}

// Face a-b-f-e
Cube::Face	Cube::back() const noexcept {
	return {{
		lowerBottomLeft(),
		lowerBottomRight(),
		upperBottomRight(),
		upperBottomLeft()
	}, FaceType::FACE_BACK
	};
	// }, {
	// 	{0.5f, 0.5f},
	// 	{0.75f, 0.5f},
	// 	{0.75f, 0.25f},
	// 	{0.5f, 0.25f}
	// }};
}

// Face c-d-h-g
Cube::Face	Cube::front() const noexcept {
	return {{
		lowerTopRight(),
		lowerTopLeft(),
		upperTopLeft(),
		upperTopRight()
	}, FaceType::FACE_FRONT
	};
	// }, {
	// 	{0.0f, 0.5f},
	// 	{0.25f, 0.5f},
	// 	{0.25f, 0.25f},
	// 	{0.0f, 0.25f}
	// }};
}

} // namespace vox