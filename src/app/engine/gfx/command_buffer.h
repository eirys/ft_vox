/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_buffer.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 08:34:43 by etran             #+#    #+#             */
/*   Updated: 2023/11/16 21:59:16 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# include <vulkan/vulkan.h>

namespace scop::core {
class Device;
}

namespace scop::gfx {

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
		scop::core::Device& device,
		VkCommandPool pool,
		uint32_t count = 1);
	void				destroy(scop::core::Device& device, VkCommandPool pool);

	/* ========================================================================= */

	void				begin(
		VkCommandBufferUsageFlags flags =
			VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
	void				restart(
		scop::core::Device& device,
		VkCommandBufferUsageFlags flags =
			VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
	void				reset();
	void				end(scop::core::Device& device, bool await = true);

	/* ========================================================================= */

	VkCommandBuffer		getBuffer() const noexcept;
	operator VkCommandBuffer() const noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	VkCommandBuffer		_buffer;

}; // class CommandBuffer

} // namespace scop::gfx