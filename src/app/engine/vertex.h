/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vertex.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:17:01 by etran             #+#    #+#             */
/*   Updated: 2023/09/22 15:16:15 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# include <vulkan/vulkan.h>

// Std
# include <array>
# include <cmath>

namespace vox {

struct Vertex {
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	uint32_t	packed_position{};

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	Vertex(uint8_t x, uint8_t y, uint8_t z): packed_position((x << 16) | (y << 8) | z) {}

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
	 * @brief Vertex format description.
	*/
	static std::array<VkVertexInputAttributeDescription, 1>	getAttributeDescriptions() {
		VkVertexInputAttributeDescription	attribute{};
		attribute.binding = 0;
		attribute.location = 0;
		attribute.format = VK_FORMAT_R32_SINT;
		attribute.offset = offsetof(Vertex, packed_position);

		return std::array<VkVertexInputAttributeDescription, 1>{ attribute };
	}

}; // struct Vertex

} // namespace vox