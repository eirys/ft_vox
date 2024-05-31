/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bounding_frustum.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 19:39:12 by etran             #+#    #+#             */
/*   Updated: 2024/05/31 15:08:39 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "types.h"
#include "vector.h"

namespace ui {
struct Camera;
} // namespace ui

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

    BoundingFrustum(const ui::Camera& cam);

    BoundingFrustum(): m_planes() {}

    BoundingFrustum(BoundingFrustum&& other) = default;
    BoundingFrustum(const BoundingFrustum& other) = default;
    BoundingFrustum& operator=(BoundingFrustum&& other) = default;
    BoundingFrustum& operator=(const BoundingFrustum& other) = default;
    ~BoundingFrustum() = default;

}; // class BoundingFrustum

} // namespace vox::gfx
