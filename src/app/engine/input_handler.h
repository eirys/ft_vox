/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:15:01 by etran             #+#    #+#             */
/*   Updated: 2023/08/15 19:25:26 by etran            ###   ########.fr       */
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

class InputHandler {
public:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	InputHandler() = default;
	~InputHandler() = default;
	InputHandler(InputHandler&& x) = default;
	InputHandler&	operator=(InputHandler&& x) = default;

	InputHandler(const InputHandler& x) = delete;
	InputHandler&	operator=(const InputHandler& x) = delete;

	/* ========================================================================= */

	void			init(
		Device& device,
		const std::vector<Vertex>& vertices,
		const std::vector<uint32_t>& indices
	);
	void			destroy(Device& device);

	/* ========================================================================= */

	uint32_t		getIndicesCount() const noexcept;
	const Buffer&	getVertexBuffer() const noexcept;
	const Buffer&	getIndexBuffer() const noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	uint32_t		_indices_count;
	Buffer			_vertex_buffer;
	Buffer			_index_buffer;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void			_createVertexBuffer(
		Device& device,
		const std::vector<Vertex>& vertices
	);
	void			_createIndexBuffer(
		Device& device,
		const std::vector<uint32_t>& indices
	);

}; // class InputHandler

} // namespace scop::graphics