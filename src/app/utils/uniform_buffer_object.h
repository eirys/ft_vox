/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uniform_buffer_object.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:17:15 by etran             #+#    #+#             */
/*   Updated: 2023/07/03 11:52:26 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# define __ALIGNMENT_MAT4 16
# define __ALIGNMENT_VEC3 16
# define __ALIGNMENT_SCAL 4
# define __ALIGNMENT_BUFF 64

# include "matrix.h"

namespace scop {

struct UniformBufferObject {
public:
	/* ========================================================================= */
	/*                               HELPER OBJECTS                              */
	/* ========================================================================= */

	struct Camera {
		alignas(__ALIGNMENT_MAT4) scop::Mat4	view;
		alignas(__ALIGNMENT_MAT4) scop::Mat4	proj;
	};

	struct Light {
		alignas(__ALIGNMENT_VEC3) scop::Vect3	ambient_color;
		alignas(__ALIGNMENT_VEC3) scop::Vect3	light_vector;
		alignas(__ALIGNMENT_VEC3) scop::Vect3	light_color;
		alignas(__ALIGNMENT_SCAL) float			light_intensity;
	};

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
	alignas(__ALIGNMENT_BUFF) Light			light;

};

} // namespace scop