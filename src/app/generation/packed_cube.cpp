/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packed_cube.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 15:10:33 by etran             #+#    #+#             */
/*   Updated: 2023/09/22 19:13:10 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "packed_cube.h"
#include "vertex.h"

namespace vox {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

PackedCube::PackedCube(uint8_t x, uint8_t z): x(x), z(z) {}

/* ========================================================================== */

using UnpackedVertex = vox::PackedCube::UnpackedVertex;

UnpackedVertex	PackedCube::a() const noexcept {
	return { x, 0, z };
}

UnpackedVertex	PackedCube::b() const noexcept {
	return { uint8_t(x + 1), 0, z };
}

UnpackedVertex	PackedCube::c() const noexcept {
	return { uint8_t(x + 1), 0, uint8_t(z + 1) };
}

UnpackedVertex	PackedCube::d() const noexcept {
	return { x, 0, uint8_t(z + 1) };
}

UnpackedVertex	PackedCube::e() const noexcept {
	return { x, 1, z };
}

UnpackedVertex	PackedCube::f() const noexcept {
	return { uint8_t(x + 1), 1, z };
}

UnpackedVertex	PackedCube::g() const noexcept {
	return { uint8_t(x + 1), 1, uint8_t(z + 1) };
}

UnpackedVertex	PackedCube::h() const noexcept {
	return { x, 1, uint8_t(z + 1) };
}

/* ========================================================================== */

using Face = vox::PackedCube::Face;

/**
 * @note The vertices are set counter clockwise, seen from the outside.
*/

Face	PackedCube::top() const noexcept {
	return { e(), f(), g(), h() };
}

Face	PackedCube::bottom() const noexcept {
	return { a(), d(), c(), b() };
}

Face	PackedCube::left() const noexcept {
	return { d(), a(), e(), h() };
}

Face	PackedCube::right() const noexcept {
	return { b(), c(), g(), f() };
}

Face	PackedCube::back() const noexcept {
	return { a(), b(), f(), e() };
}

Face	PackedCube::front() const noexcept {
	return { c(), d(), h(), g() };
}

} // namespace vox
