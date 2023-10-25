/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bounding_box.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 14:14:11 by etran             #+#    #+#             */
/*   Updated: 2023/10/25 17:38:36 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bounding_box.h"

namespace scop::graphics {

/* ========================================================================== */
/*                                  PROTECTED                                 */
/* ========================================================================== */

BoundingBox::BoundingBox(Vect3 center, Vect3 half_diag):
	_half_diag(half_diag),
	_center(center) {}

IntersectionType	BoundingBox::checkPlaneIntersection(const Plane& plane) const {
	// Length of diag projected on plane normal
	float extent = _half_diag.x * std::abs(plane.x) +
				   _half_diag.y * std::abs(plane.y) +
				   _half_diag.z * std::abs(plane.z);
	float signedDistance = scop::dot(_center, plane.xyz) + plane.w;

	if (signedDistance + extent < 0)
		return IntersectionType::Outside;
	else if (signedDistance - extent > 0)
		return IntersectionType::Inside;
	return IntersectionType::Intersecting;
}

} // namespace scop::graphics