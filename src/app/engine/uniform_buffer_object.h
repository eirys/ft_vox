/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uniform_buffer_object.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:17:15 by etran             #+#    #+#             */
/*   Updated: 2023/10/29 02:38:28 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "constant_buffer_macros.h"
# include "chunk_macros.h"
# include "matrix.h"

namespace scop {

/* ========================================================================== */
/*                               HELPER OBJECTS                               */
/* ========================================================================== */

struct Camera {
	alignas(__ALIGNMENT_MAT4) scop::Mat4	vp; // view proj
};

struct Light {
	alignas(__ALIGNMENT_VEC3) scop::Vect3	ambient_color;
	alignas(__ALIGNMENT_VEC3) scop::Vect3	light_vector;
	alignas(__ALIGNMENT_VEC3) scop::Vect3	light_color;
};

/**
 * @brief Constant buffer object.
*/
struct UniformBufferObject {
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	using Camera = ::scop::Camera;
	using Projector = Camera;
	using Light = ::scop::Light;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	UniformBufferObject() = default;
	UniformBufferObject(UniformBufferObject&& other) = default;
	UniformBufferObject(const UniformBufferObject& other) = default;
	UniformBufferObject& operator=(UniformBufferObject&& other) = default;
	UniformBufferObject& operator=(const UniformBufferObject& other) = default;
	~UniformBufferObject() = default;

	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	alignas(__ALIGNMENT_BUFF) Camera		camera;
	alignas(__ALIGNMENT_BUFF) Projector		projector;
	alignas(__ALIGNMENT_BUFF) Light			light;
	alignas(__ALIGNMENT_BUFF) uint32_t		chunks[PACKED_DATA_COUNT];

}; // struct UniformBufferObject

	// static const constexpr offset0 = offsetof(UniformBufferObject, camera);
	// static const constexpr offset1 = offsetof(UniformBufferObject, projector);
	// static const constexpr offset2 = offsetof(UniformBufferObject, light);
	// static const constexpr offset3 = offsetof(UniformBufferObject, chunks);


} // namespace scop