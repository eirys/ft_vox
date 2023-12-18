/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bounding_object.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 18:09:12 by etran             #+#    #+#             */
/*   Updated: 2023/10/17 18:09:12 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "vector.h"

namespace scop::gfx {

struct BoundingFrustum;

enum class IntersectionType: uint8_t {
	Outside,
	Inside,
	Intersecting
};

class IBoundingObject {
public:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	virtual ~IBoundingObject() = default;

protected:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	IBoundingObject() = default;
	IBoundingObject(const IBoundingObject& other) = default;
	IBoundingObject(IBoundingObject&& other) = default;
	IBoundingObject& operator=(const IBoundingObject& other) = default;
	IBoundingObject& operator=(IBoundingObject&& other) = default;

	/* ========================================================================= */

	virtual IntersectionType	_checkIntersection(
		const BoundingFrustum& frustum) const = 0;

};

} // namespace scop::gfx
