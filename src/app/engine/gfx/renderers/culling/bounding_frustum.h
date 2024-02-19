/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bounding_frustum.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 16:25:31 by etran             #+#    #+#             */
/*   Updated: 2024/01/20 12:55:47 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "vector.h"
# include "chunk_macros.h"

namespace scop::gfx {

// Resource:
// https://iquilezles.org/articles/frustum/
// https://iquilezles.org/articles/frustumcorrect/

struct BoundingFrustum {
	/* ========================================================================= */
	/*                                HELPER CLASS                               */
	/* ========================================================================= */

	// xyz: normal
	// w:   d (ax+by+cz+d)
	using Plane = ::scop::Vect4;

	struct Camera {
		const scop::Vect3&	position;
		const scop::Vect3&	front;
		const scop::Vect3&	right;
		const scop::Vect3&	up;
	};

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	BoundingFrustum();
	BoundingFrustum(const Camera& cam);

	BoundingFrustum(BoundingFrustum&& other) = default;
	BoundingFrustum(const BoundingFrustum& other) = default;
	BoundingFrustum& operator=(BoundingFrustum&& other) = default;
	BoundingFrustum& operator=(const BoundingFrustum& other) = default;
	~BoundingFrustum() = default;

	/* ========================================================================= */
	/*                                    DATA                                   */
	/* ========================================================================= */

	union {
		struct {
			Plane	far;
			Plane	near;
			Plane	top;
			Plane	bottom;
			Plane	left;
			Plane	right;
		};
		Plane	planes[FACE_COUNT];
	};

}; // struct BoundingFrustum

} // namespace scop::gfx