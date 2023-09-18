/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uniform_buffer_object.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:17:15 by etran             #+#    #+#             */
/*   Updated: 2023/09/01 16:13:21 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# define __ALIGNMENT_MAT4 16
# define __ALIGNMENT_VEC3 16
# define __ALIGNMENT_SCAL 4
# define __ALIGNMENT_BUFF 64

# include "matrix.h"

namespace scop {

/* ========================================================================== */
/*                               HELPER OBJECTS                               */
/* ========================================================================== */

struct Camera {
	alignas(__ALIGNMENT_MAT4) scop::Mat4	vp;
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

};

} // namespace scop