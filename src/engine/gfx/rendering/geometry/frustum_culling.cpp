/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frustum_culling.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:07:11 by etran             #+#    #+#             */
/*   Updated: 2024/08/26 13:20:52 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bounding_box.h"
#include "bounding_frustum.h"
#include "maths.h"
#include "camera.h"

namespace vox::gfx {

    /* ====================================================================== */
    /*                             BOUNDINGFRUSTUM                            */
    /* ====================================================================== */

BoundingFrustum::BoundingFrustum(const game::Camera& cam) {
    const float halfHeight = cam.getSettings().farPlane * tanf(math::radians(cam.getSettings().fov) * .5f);
    const float halfWidth = halfHeight * game::Camera::ASPECT_RATIO;

    const math::vec3 nearFront = cam.getSettings().nearPlane * cam.m_directions.front;
    const math::vec3 farFront = cam.getSettings().farPlane * cam.m_directions.front;

    m_near.xyz = cam.m_directions.front;
    m_near.w = math::dot(m_near.xyz, cam.m_position + nearFront);

    m_far.xyz = -cam.m_directions.front;
    m_far.w = math::dot(m_far.xyz, cam.m_position + farFront);

    m_right.xyz = math::normalize(math::cross(cam.m_directions.up, farFront + cam.m_directions.right * halfWidth));
    m_right.w = math::dot(m_right.xyz, cam.m_position);

    m_left.xyz = math::normalize(math::cross(farFront - cam.m_directions.right * halfWidth, cam.m_directions.up));
    m_left.w = math::dot(m_left.xyz, cam.m_position);

    m_top.xyz = math::normalize(math::cross(farFront + cam.m_directions.up * halfHeight, cam.m_directions.right));
    m_top.w = math::dot(m_top.xyz, cam.m_position);

    m_bottom.xyz = math::normalize(math::cross(cam.m_directions.right, farFront - cam.m_directions.up * halfHeight));
    m_bottom.w = math::dot(m_bottom.xyz, cam.m_position);
}

    /* ====================================================================== */
    /*                               BOUNDINGBOX                              */
    /* ====================================================================== */

BoundingBox::BoundingBox(const math::vec3& center, const math::vec3& halfExtent):
    m_center(center),
    m_halfExtent(halfExtent) {}

/* ========================================================================== */

bool BoundingBox::isVisible(const BoundingFrustum& frustum) const {
    for (const math::vec4& plane: frustum.m_planes) {
        if (!_isInsidePlane(plane)) return false;
    }
    return true;
}

bool BoundingBox::_isInsidePlane(const math::vec4& plane) const {
    // Length of diag projected on plane normal
    const float extent = math::dot(m_halfExtent, abs(plane.xyz));
    const float signedDistance = math::dot(m_center, plane.xyz) - plane.w;

    if (signedDistance + extent < 0.0f)
        return false;
    return true;
}

} // namespace vox::gfx