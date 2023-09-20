/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_buffer.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 08:34:43 by etran             #+#    #+#             */
/*   Updated: 2023/08/11 23:51:40 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# ifndef GLFW_INCLUDE_VULKAN
#  define GLFW_INCLUDE_VULKAN
# endif

# include <GLFW/glfw3.h>

namespace scop::graphics {
class Device;
class CommandPool;

/**
 * @brief Wrapper class for VkCommandBuffer.
*/
class CommandBuffer final {
public:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	CommandBuffer() = default;
	~CommandBuffer() = default;
	CommandBuffer(CommandBuffer&& other) = default;
	CommandBuffer& operator=(CommandBuffer&& other) = default;

	CommandBuffer(const CommandBuffer& other) = delete;
	CommandBuffer& operator=(const CommandBuffer& other) = delete;

	/* ========================================================================= */

	void				init(
		Device& device,
		VkCommandPool pool,
		uint32_t count = 1);
	void				destroy(Device& device, VkCommandPool pool);

	/* ========================================================================= */

	void				begin(
		VkCommandBufferUsageFlags flags =
			VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
	void				restart(
		Device& device,
		VkCommandBufferUsageFlags flags =
			VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
	void				reset();
	void				end(Device& device, bool await = true);

	/* ========================================================================= */

	VkCommandBuffer		getBuffer() const noexcept;
	operator VkCommandBuffer() const noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	VkCommandBuffer		_buffer;

}; // class CommandBuffer

} // namespace scop::graphics