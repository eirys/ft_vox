/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vertex_input.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:15:01 by etran             #+#    #+#             */
/*   Updated: 2023/06/23 16:30:52 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# ifndef GLFW_INCLUDE_VULKAN
#  define GLFW_INCLUDE_VULKAN
# endif

# include <GLFW/glfw3.h>

// Std
# include <vector>

# include "vertex.h"
# include "vector.h"
# include "buffer.h"

namespace scop {
namespace graphics {
class Engine;
class Device;

class VertexInput {
public:
	friend Engine;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	VertexInput() = default;
	VertexInput(VertexInput&& x) = default;
	~VertexInput() = default;

	VertexInput(const VertexInput& x) = delete;
	VertexInput&	operator=(VertexInput&& x) = delete;

	/* ========================================================================= */

	void	init(
		Device& device,
		VkCommandPool command_pool,
		const std::vector<Vertex>& vertices,
		const std::vector<uint32_t>& indices
	);
	void	destroy(Device& device);

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	Buffer	vertex_buffer;
	Buffer	index_buffer;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void	createVertexBuffer(
		Device& device,
		VkCommandPool command_pool,
		const std::vector<Vertex>& vertices
	);
	void	createIndexBuffer(
		Device& device,
		VkCommandPool command_pool,
		const std::vector<uint32_t>& indices
	);

}; // class VertexInput

} // namespace graphics
} // namespace scop
