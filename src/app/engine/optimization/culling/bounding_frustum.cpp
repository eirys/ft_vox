/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bounding_frustum.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 12:16:44 by etran             #+#    #+#             */
/*   Updated: 2023/10/25 18:06:18 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bounding_frustum.h"
#include "scop_math.h"
#include "player.h"

namespace scop::graphics {

// BoundingFrustum::BoundingFrustum(float fov, float ratio, float znear, float zfar) {
// 	const float angle = scop::math::radians(fov);
// 	const float si = sinf(angle);
// 	const float co = cosf(angle);

// 	// Planes pass through origin. Just update normal vectors.
// 	top.xyz		= { 0.0f, -co, -si };
// 	bottom.xyz	= { 0.0f, co, -si };

// 	left.xyz	= scop::normalize({ co, 0.0f, -si * ratio });
// 	right.xyz	= scop::normalize({ -co, 0.0f, -si * ratio });

// 	far			= { 0.0f, 0.0f, -1.0f, -zfar };
// 	near		= { 0.0f, 0.0f, 1.0f, znear };
// }

BoundingFrustum::BoundingFrustum(): planes{} {}

BoundingFrustum::BoundingFrustum(const Camera& cam) {
	const float half_height = CAMERA_ZFAR * tanf(PLAYER_FOV * .5f);
	const float half_width = half_height * WINDOW_RATIO;

	const scop::Vect3 near_front = CAMERA_ZNEAR * cam.front;
	const scop::Vect3 far_front = CAMERA_ZFAR * cam.front;

	near.xyz = cam.front;
	near.w = -scop::dot(near.xyz, cam.position + near_front);

	far.xyz = -cam.front;
	far.w = -scop::dot(far.xyz, cam.position + far_front);

	right.xyz = scop::normalize(scop::cross(far_front - cam.right * half_width, cam.up));
	right.w = -scop::dot(right.xyz, cam.position);

	left.xyz = scop::normalize(scop::cross(cam.up, far_front + cam.right * half_width));
	left.w = -scop::dot(left.xyz, cam.position);

	top.xyz = scop::normalize(scop::cross(cam.right, far_front - cam.up * half_height));
	top.w = -scop::dot(top.xyz, cam.position);

	bottom.xyz = scop::normalize(scop::cross(far_front + cam.up * half_height, cam.right));
	bottom.w = -scop::dot(bottom.xyz, cam.position);
}

} // namespace scop::graphics