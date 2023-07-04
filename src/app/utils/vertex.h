/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vertex.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:17:01 by etran             #+#    #+#             */
/*   Updated: 2023/07/04 22:24:50 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# include <vulkan/vulkan.h>

// Std
# include <array>

# include "vector.h"

namespace scop {

struct Vertex {
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	scop::Vect3		pos;
	scop::Vect2		uv;
	scop::Vect3		normal;
	int32_t			texture_id;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	Vertex() = default;
	Vertex(Vertex&&) = default;
	Vertex(const Vertex&) = default;
	Vertex&	operator=(Vertex&&) = default;
	Vertex&	operator=(const Vertex&) = default;
	~Vertex() = default;

	/* HELPER FUNCTIONS ======================================================== */

	/**
	 * Expliciting to vulkan how to pass data to vertex shader.
	*/
	static VkVertexInputBindingDescription	getBindingDescription() {
		VkVertexInputBindingDescription	binding_description{};

		binding_description.binding = 0;
		binding_description.stride = sizeof(Vertex);
		binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return binding_description;
	}

	/**
	 * Expliciting to vulkan the vertex struct format.
	*/
	static std::array<VkVertexInputAttributeDescription, 4>	getAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 4>	attribute_descriptions{};

		// `pos` attribute
		attribute_descriptions[0].binding = 0;
		attribute_descriptions[0].location = 0;
		attribute_descriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attribute_descriptions[0].offset = offsetof(Vertex, pos);

		// `uv` attribute
		attribute_descriptions[1].binding = 0;
		attribute_descriptions[1].location = 1;
		attribute_descriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
		attribute_descriptions[1].offset = offsetof(Vertex, uv);

		// `normal` attribute
		attribute_descriptions[2].binding = 0;
		attribute_descriptions[2].location = 2;
		attribute_descriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
		attribute_descriptions[2].offset = offsetof(Vertex, normal);

		// `texture_id` attribute
		attribute_descriptions[3].binding = 0;
		attribute_descriptions[3].location = 3;
		attribute_descriptions[3].format = VK_FORMAT_R32_SINT;
		attribute_descriptions[3].offset = offsetof(Vertex, texture_id);

		return attribute_descriptions;
	}

	bool	operator==(const Vertex& rhs) const {
		return
			pos == rhs.pos &&
			uv == rhs.uv &&
			normal == rhs.normal &&
			texture_id == rhs.texture_id;
	}
}; // struct Vertex

} // namespace scop

template<>
struct std::hash<scop::Vertex> {
	std::size_t	operator()(const scop::Vertex& vertex) const {
		return
			(std::hash<scop::Vect3>()(vertex.pos)) ^
			(std::hash<scop::Vect2>()(vertex.uv)) ^
			(std::hash<scop::Vect3>()(vertex.normal)) ^
			(std::hash<float>()(vertex.texture_id));
	}
};