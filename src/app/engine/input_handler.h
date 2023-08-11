/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_buffer.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:15:01 by etran             #+#    #+#             */
/*   Updated: 2023/08/10 22:13:08 by etran            ###   ########.fr       */
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
} // namespace scop

namespace scop::graphics {
class Device;
class CommandPool;

class InputHandler {
public:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	InputHandler() = default;
	~InputHandler() = default;

	InputHandler(InputHandler&& x) = delete;
	InputHandler(const InputHandler& x) = delete;
	InputHandler&	operator=(const InputHandler& x) = delete;
	InputHandler&	operator=(InputHandler&& x) = delete;

	/* ========================================================================= */

	void			init(
		Device& device,
		CommandPool& command_pool,
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
		CommandPool& command_pool,
		const std::vector<Vertex>& vertices
	);
	void			_createIndexBuffer(
		Device& device,
		CommandPool& command_pool,
		const std::vector<uint32_t>& indices
	);

}; // class InputHandler

} // namespace scop::graphics