/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bounding_frustum.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 12:16:44 by etran             #+#    #+#             */
/*   Updated: 2023/10/17 12:16:44 by etran            ###   ########.fr       */
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

BoundingFrustum::BoundingFrustum(const Camera& cam) {
	const float half_width = CAMERA_ZFAR * tanf(PLAYER_FOV * .5f);
	const float half_height = half_width * WINDOW_RATIO;

	const scop::Vect3 far_front = CAMERA_ZFAR * cam.front;

	auto computeDistanceFn = [](const Vect3& normal, const Vect3& point) -> float {
		return - ((normal.x * point.x) +
				  (normal.y * point.y) +
				  (normal.z * point.z));
	};

	near.xyz = { cam.position + CAMERA_ZNEAR * cam.front };
	near.w = computeDistanceFn(near.xyz, cam.front);

	far.xyz = { cam.position + far_front };
	far.w = computeDistanceFn(far.xyz, -cam.front);

	right.xyz = { cam.position };
	right.w = computeDistanceFn(right.xyz,  scop::cross(far_front - cam.right * half_width, cam.up));

	left.xyz = { cam.position };
	left.w = computeDistanceFn(left.xyz, scop::cross(cam.up, far_front + cam.right * half_width));

	top.xyz = { cam.position };
	top.w = computeDistanceFn(top.xyz, scop::cross(cam.right, far_front - cam.up * half_height));

	bottom.xyz = { cam.position };
	bottom.w = computeDistanceFn(bottom.xyz, scop::cross(far_front + cam.up * half_height, cam.right));

	for (auto& plane: planes)
		plane.xyz = scop::normalize(plane.xyz);
}

} // namespace scop::graphics