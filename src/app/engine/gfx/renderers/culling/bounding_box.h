/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bounding_box.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 14:12:32 by etran             #+#    #+#             */
/*   Updated: 2023/12/08 15:37:11 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "bounding_object.h"
# include "bounding_frustum.h"

namespace scop::gfx {

class BoundingBox: public IBoundingObject {
public:
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	using Plane = BoundingFrustum::Plane;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	virtual ~BoundingBox() = default;

protected:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	BoundingBox(Vect3 center, Vect3 half_diag);

	BoundingBox() = default;
	BoundingBox(const BoundingBox& other) = default;
	BoundingBox(BoundingBox&& other) = default;
	BoundingBox& operator=(const BoundingBox& other) = default;
	BoundingBox& operator=(BoundingBox&& other) = default;

	/* ========================================================================= */

	IntersectionType	_checkPlaneIntersection(const Plane& plane) const;

	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	Vect3	_half_diag;
	Vect3	_center;

}; // class BoundingBox

} // namespace scop::gfx