/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bounding_box.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 19:14:49 by etran             #+#    #+#             */
/*   Updated: 2024/05/30 22:22:16 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "vector.h"
#include "bounding_frustum.h"

namespace vox::gfx {

enum class IntersectionType {
    Outside,
    Inside,
    Intersecting
};

class BoundingBox final {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    BoundingBox(const math::Vect3& center, const math::Vect3& halfExtent):
        m_center(center),
        m_halfExtent(halfExtent) {}

    BoundingBox() = default;
    ~BoundingBox() = default;
    BoundingBox(BoundingBox&& other) = default;
    BoundingBox(const BoundingBox& other) = default;
    BoundingBox& operator=(BoundingBox&& other) = default;
    BoundingBox& operator=(const BoundingBox& other) = default;

    /* ====================================================================== */

    IntersectionType    checkIntersection(const BoundingFrustum& frustum) const {
        bool isIntersecting = false;

        for (const math::Vect4& plane: frustum.m_planes) {
            IntersectionType planeIntersection = _checkPlaneIntersection(plane);

            if (planeIntersection == IntersectionType::Outside)
                return IntersectionType::Outside;
            else if (planeIntersection == IntersectionType::Intersecting)
                isIntersecting = true;
        }

        if (isIntersecting)
            return IntersectionType::Intersecting;
        return IntersectionType::Inside;
    }

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    math::Vect3 m_center;
    math::Vect3 m_halfExtent;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    IntersectionType _checkPlaneIntersection(const math::Vect4& plane) const {
        // Length of diag projected on plane normal
        const float extent = math::dot(m_halfExtent, abs(plane.xyz));
        const float signedDistance = math::dot(m_center, plane.xyz) + plane.w;

        if (signedDistance + extent < 0.0f)
            return IntersectionType::Outside;
        else if (signedDistance - extent > 0.0f)
            return IntersectionType::Inside;
        return IntersectionType::Intersecting;
    }

}; // class BoundingBox

} // namespace vox::gfx
