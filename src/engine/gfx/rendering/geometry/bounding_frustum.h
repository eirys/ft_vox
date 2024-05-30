/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bounding_frustum.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 19:39:12 by etran             #+#    #+#             */
/*   Updated: 2024/05/30 22:26:06 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "types.h"
#include "vector.h"

namespace vox::gfx {

/**
 * @brief A bounding frustum is a geometric shape that defines the volume of space that is visible to the camera.
 *
 * @note https://iquilezles.org/articles/frustum/
 * @note https://iquilezles.org/articles/frustumcorrect/
 */
struct BoundingFrustum final {
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    union {
        struct {
            math::Vect4 m_far;
            math::Vect4 m_near;
            math::Vect4 m_top;
            math::Vect4 m_bottom;
            math::Vect4 m_left;
            math::Vect4 m_right;
        };
        math::Vect4 m_planes[6];
    };

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    BoundingFrustum(const ui::Camera& cam) {
        const float halfHeight = ui::Camera::FAR_PLANE * tanf(cam.m_fov * .5f);
        const float halfWidth = halfHeight * ui::Camera::ASPECT_RATIO;
        const math::Vect3 nearFront = ui::Camera::NEAR_PLANE * cam.m_front;
        const math::Vect3 farFront = ui::Camera::FAR_PLANE * cam.m_front;

        m_near.xyz = cam.m_front;
        m_near.w = -math::dot(m_near.xyz, cam.m_position + nearFront);

        m_far.xyz = -cam.m_front;
        m_far.w = -math::dot(m_far.xyz, cam.m_position + farFront);

        m_right.xyz = math::normalize(math::cross(cam.m_up, farFront + (cam.m_right * halfWidth)));
        m_right.w = -math::dot(m_right.xyz, cam.m_position);

        m_left.xyz = math::normalize(math::cross(farFront - (cam.m_right * halfWidth), cam.m_up));
        m_left.w = -math::dot(m_left.xyz, cam.m_position);

        m_top.xyz = math::normalize(math::cross(cam.m_right, farFront - (cam.m_up * halfHeight)));
        m_top.w = -math::dot(m_top.xyz, cam.m_position);

        m_bottom.xyz = math::normalize(math::cross(farFront + (cam.m_up * halfHeight), cam.m_right));
        m_bottom.w = -math::dot(m_bottom.xyz, cam.m_position);
    }

    BoundingFrustum(): m_planes() {}

    BoundingFrustum(BoundingFrustum&& other) = default;
    BoundingFrustum(const BoundingFrustum& other) = default;
    BoundingFrustum& operator=(BoundingFrustum&& other) = default;
    BoundingFrustum& operator=(const BoundingFrustum& other) = default;
    ~BoundingFrustum() = default;

}; // class BoundingFrustum

} // namespace vox::gfx
