/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packed_cube.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 15:10:33 by etran             #+#    #+#             */
/*   Updated: 2023/09/22 11:20:19 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <cstdint>
# include <array> // std::array

namespace vox {

/**
 * @brief GFX Representation of a Block but with packed position data.
 *
 * @note Its position is represented by the bottom left corner of its lower face.
 *
 *   h_____g    Frame of reference:
 * e/|___f/|    y z
 * | d---|-c    |/
 * |/    |/     O___x
 * a_____b
*/
struct PackedCube final {
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	struct UnpackedVertex {
		uint8_t x;
		uint8_t y;
		uint8_t z;
	};

	/**
	 * @brief Contains the vertices of a face, counter clockwise. (cf. drawing)
	*/
	struct Face {
		std::array<UnpackedVertex, 4>	vertices;
	};

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	PackedCube(uint8_t x, uint8_t z);

	PackedCube() = default;
	PackedCube(PackedCube&& x) = default;
	PackedCube(const PackedCube& x) = default;
	PackedCube& operator=(PackedCube&& x) = default;
	PackedCube& operator=(const PackedCube& x) = default;
	~PackedCube() = default;

	/* VERTICES ================================================================ */

	UnpackedVertex		a() const noexcept;
	UnpackedVertex		b() const noexcept;
	UnpackedVertex		c() const noexcept;
	UnpackedVertex		d() const noexcept;
	UnpackedVertex		e() const noexcept;
	UnpackedVertex		f() const noexcept;
	UnpackedVertex		g() const noexcept;
	UnpackedVertex		h() const noexcept;

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

	uint8_t				x;
	uint8_t				z;

}; // class PackedCube

} // namespace vox