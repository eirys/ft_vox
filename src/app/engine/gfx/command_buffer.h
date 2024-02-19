/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_buffer.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 08:34:43 by etran             #+#    #+#             */
/*   Updated: 2024/01/20 12:55:47 by etran            ###   ########.fr       */
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

enum class CommandBufferType: uint8_t {
	DRAW,
	COMPUTE,

	UNKNOWN
};

/**
 * @brief Wrapper class for VkCommandBuffer.
*/
class CommandBuffer final {
public:
	friend CommandPool;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	CommandBuffer(CommandBufferType type);

	CommandBuffer() = default;
	~CommandBuffer() = default;
	CommandBuffer(CommandBuffer&& other) = default;
	CommandBuffer& operator=(CommandBuffer&& other) = default;

	CommandBuffer(const CommandBuffer& other) = delete;
	CommandBuffer& operator=(const CommandBuffer& other) = delete;

	/* ========================================================================= */

	void				begin(
		VkCommandBufferUsageFlags flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
	void				reset();
	void				end(scop::core::Device& device, bool await = true);

	/* ========================================================================= */

	VkCommandBuffer		getBuffer() const noexcept;
	CommandBufferType	getType() const noexcept;

private:
	/* ========================================================================= */
	/*                                    DATA                                   */
	/* ========================================================================= */

	VkCommandBuffer		_buffer = VK_NULL_HANDLE;
	CommandBufferType	_type = CommandBufferType::UNKNOWN;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void				init(scop::core::Device& device);
	void				destroy(scop::core::Device& device);

}; // class CommandBuffer

} // namespace scop::gfx