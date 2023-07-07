/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vertex.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:17:01 by etran             #+#    #+#             */
/*   Updated: 2023/07/07 16:55:37 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# include <vulkan/vulkan.h>

// Std
# include <vector>
# include <cmath>

# include "vector.h"
# include "chunk.hpp"

namespace scop {

struct Vertex {
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	int32_t	pos{};		// Vertex position
	int32_t	n_uv_f{};	// Normal, UV and Face index

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	Vertex(
		scop::Vect3 pos,
		uint8_t normal,
		uint8_t uv,
		uint8_t index
	):	pos(vox::toChunkPos(pos.x, pos.y, pos.z)),
		n_uv_f(
			vox::convert(normal) |
			vox::convert(uv, 8) |
			vox::convert(index, 16)
		) {}

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
	static std::vector<VkVertexInputAttributeDescription>	getAttributeDescriptions() {
		std::vector<VkVertexInputAttributeDescription>	attribute_descriptions;
		attribute_descriptions.reserve(2);

		VkVertexInputAttributeDescription	attribute{};
		attribute.binding = 0;

		// `pos` attribute
		attribute.location = 0;
		attribute.format = VK_FORMAT_R32_SINT;
		attribute.offset = offsetof(Vertex, pos);
		attribute_descriptions.emplace_back(attribute);

		// `normal_index`, `uv_index` and `face_index` attribute
		attribute.location = 1;
		attribute.format = VK_FORMAT_R32_SINT;
		attribute.offset = offsetof(Vertex, n_uv_f);
		attribute_descriptions.emplace_back(attribute);

		return attribute_descriptions;
	}

	// int32_t	convertPos(const Vect3& pos) const noexcept {
	// 	float	x = pos.x > 0 ? pos.x : CHUNK_SIZE - pos.x;
	// 	float	z = pos.z > 0 ? pos.z : CHUNK_SIZE - pos.z;

	// 	int16_t chunk_address =
	// 		((int)z / CHUNK_SIZE) * DEFAULT_RENDER_DISTANCE +
	// 		(int)x / CHUNK_SIZE;

	// 	x = (int)x % CHUNK_SIZE;
	// 	z = (int)z % CHUNK_SIZE;

	// 	return
	// 		convert(x) |
	// 		convert(pos.y, 8) |
	// 		convert(z, 16) |
	// 		convert(chunk_address, 24);
	// }

	bool	operator==(const Vertex& rhs) const {
		return
			pos == rhs.pos &&
			n_uv_f == rhs.n_uv_f;
	}

}; // struct Vertex

} // namespace scop

template<>
struct std::hash<scop::Vertex> {
	std::size_t	operator()(const scop::Vertex& vertex) const {
		return
			(std::hash<uint32_t>()(vertex.pos)) ^
			(std::hash<uint32_t>()(vertex.n_uv_f));
	}
};