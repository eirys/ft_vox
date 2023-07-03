/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_buffer.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:15:01 by etran             #+#    #+#             */
/*   Updated: 2023/07/03 10:21:18 by etran            ###   ########.fr       */
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

# include "buffer.h"

namespace scop {
struct Vertex;

namespace graphics {
class Device;

class InputBuffer {
public:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	InputBuffer() = default;
	~InputBuffer() = default;

	InputBuffer(InputBuffer&& x) = delete;
	InputBuffer(const InputBuffer& x) = delete;
	InputBuffer&	operator=(const InputBuffer& x) = delete;
	InputBuffer&	operator=(InputBuffer&& x) = delete;

	/* ========================================================================= */

	void			init(
		Device& device,
		VkCommandPool command_pool,
		const std::vector<Vertex>& vertices,
		const std::vector<uint32_t>& indices
	);
	void			destroy(Device& device);

	/* ========================================================================= */

	const Buffer&	getVertexBuffer() const noexcept;
	const Buffer&	getIndexBuffer() const noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	Buffer			_vertex_buffer;
	Buffer			_index_buffer;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void			_createVertexBuffer(
		Device& device,
		VkCommandPool command_pool,
		const std::vector<Vertex>& vertices
	);
	void			_createIndexBuffer(
		Device& device,
		VkCommandPool command_pool,
		const std::vector<uint32_t>& indices
	);

}; // class InputBuffer

} // namespace graphics
} // namespace scop
