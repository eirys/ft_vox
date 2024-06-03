/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frustum_culling.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:07:11 by etran             #+#    #+#             */
/*   Updated: 2024/05/31 18:32:53 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bounding_box.h"
#include "bounding_frustum.h"
#include "maths.h"
#include "controller.h"

namespace vox::gfx {

    /* ====================================================================== */
    /*                             BOUNDINGFRUSTUM                            */
    /* ====================================================================== */

BoundingFrustum::BoundingFrustum(const ui::Camera& cam) {
    const float halfHeight = ui::Camera::FAR_PLANE * tanf(math::radians(cam.m_fov) * .5f);
    const float halfWidth = halfHeight * ui::Camera::ASPECT_RATIO;

    const math::Vect3 nearFront = ui::Camera::NEAR_PLANE * cam.m_front;
    const math::Vect3 farFront = ui::Camera::FAR_PLANE * cam.m_front;

    m_near.xyz = cam.m_front;
    m_near.w = math::dot(m_near.xyz, cam.m_position + nearFront);

    m_far.xyz = -cam.m_front;
    m_far.w = math::dot(m_far.xyz, cam.m_position + farFront);

    m_right.xyz = math::normalize(math::cross(cam.m_up, farFront + cam.m_right * halfWidth));
    m_right.w = math::dot(m_right.xyz, cam.m_position);

    m_left.xyz = math::normalize(math::cross(farFront - cam.m_right * halfWidth, cam.m_up));
    m_left.w = math::dot(m_left.xyz, cam.m_position);

    m_top.xyz = math::normalize(math::cross(farFront + cam.m_up * halfHeight, cam.m_right));
    m_top.w = math::dot(m_top.xyz, cam.m_position);

    m_bottom.xyz = math::normalize(math::cross(cam.m_right, farFront - cam.m_up * halfHeight));
    m_bottom.w = math::dot(m_bottom.xyz, cam.m_position);
}

    /* ====================================================================== */
    /*                               BOUNDINGBOX                              */
    /* ====================================================================== */

BoundingBox::BoundingBox(const math::Vect3& center, const math::Vect3& halfExtent):
    m_center(center),
    m_halfExtent(halfExtent) {}

/* ========================================================================== */

bool BoundingBox::isVisible(const BoundingFrustum& frustum) const {
    for (const math::Vect4& plane: frustum.m_planes) {
        if (!_isInsidePlane(plane)) return false;
    }
    return true;
}

bool BoundingBox::_isInsidePlane(const math::Vect4& plane) const {
    // Length of diag projected on plane normal
    const float extent = math::dot(m_halfExtent, abs(plane.xyz));
    const float signedDistance = math::dot(m_center, plane.xyz) - plane.w;

    if (signedDistance + extent < 0.0f)
        return false;
    return true;
}

} // namespace vox::gfx