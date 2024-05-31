/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bounding_box.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 19:14:49 by etran             #+#    #+#             */
/*   Updated: 2024/05/31 18:33:02 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "vector.h"

namespace vox::gfx {

class BoundingFrustum;

class BoundingBox final {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    BoundingBox(const math::Vect3& center, const math::Vect3& halfExtent);

    BoundingBox() = default;
    ~BoundingBox() = default;
    BoundingBox(BoundingBox&& other) = default;
    BoundingBox(const BoundingBox& other) = default;
    BoundingBox& operator=(BoundingBox&& other) = default;
    BoundingBox& operator=(const BoundingBox& other) = default;

    /* ====================================================================== */

    bool                isVisible(const BoundingFrustum& frustum) const;

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    math::Vect3 m_center;
    math::Vect3 m_halfExtent;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    bool    _isInsidePlane(const math::Vect4& plane) const;

}; // class BoundingBox

} // namespace vox::gfx
