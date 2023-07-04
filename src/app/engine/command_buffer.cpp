/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_buffer.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 08:39:55 by etran             #+#    #+#             */
/*   Updated: 2023/07/04 09:46:17 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_buffer.h"
#include "device.h"
#include "command_pool.h"

#include <stdexcept> // std::runtime_error

namespace scop {
namespace graphics {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

/**
 * @brief Initialize the command buffer.
*/
void	CommandBuffer::init(
	Device& device,
	CommandPool& pool,
	uint32_t count
) {
	VkCommandBufferAllocateInfo	alloc{};
	alloc.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	alloc.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	alloc.commandPool = pool.getPool();
	alloc.commandBufferCount = count;

	if (vkAllocateCommandBuffers(device.getLogicalDevice(), &alloc, &_buffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffer");
	}
}

/**
 * @brief Deallocate the command buffer.
*/
void	CommandBuffer::destroy(
	Device& device,
	CommandPool& pool
) {
	vkFreeCommandBuffers(
		device.getLogicalDevice(),
		pool.getPool(),
		1, &_buffer
	);
}

/* ========================================================================== */

/**
 * @brief Starts recording the command buffer.
 * 
 * @param flags Command buffer usage flags.
 * Defaults to VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT.
*/
void	CommandBuffer::begin(
	VkCommandBufferUsageFlags flags
) {
	VkCommandBufferBeginInfo	begin_info{};
	begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	begin_info.flags = flags;

	if (vkBeginCommandBuffer(_buffer, &begin_info) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer");
	}
}

/**
 * @brief Reset the command buffer.
*/
void	CommandBuffer::reset() {
	vkResetCommandBuffer(_buffer, 0);
}

/**
 * @brief Restart the command buffer.
 * 
 * @param flags Command buffer usage flags.
 * Defaults to VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT.
*/
void	CommandBuffer::restart(
	Device& device,
	VkCommandBufferUsageFlags flags
) {
	end(device);
	reset();
	begin(flags);
}

/**
 * @brief Ends recording the command buffer.
 * 
 * @note A fence is created to wait for the transfer to complete.
*/
void	CommandBuffer::end(Device& device) {
	if (vkEndCommandBuffer(_buffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer");
	}

	// Create fence to wait for transfer to complete before deallocating
	VkFence				fence;
	VkFenceCreateInfo	fence_info{};
	fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	if (vkCreateFence(device.getLogicalDevice(), &fence_info, nullptr, &fence) != VK_SUCCESS) {
		throw std::runtime_error("failed to create fence for command buffer");
	}

	VkSubmitInfo	submit_info{};
	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &_buffer;
	if (vkQueueSubmit(device.getGraphicsQueue(), 1, &submit_info, fence) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit command buffer to queue");
	}

	vkWaitForFences(device.getLogicalDevice(), 1, &fence, VK_TRUE, UINT64_MAX);
	vkDestroyFence(device.getLogicalDevice(), fence, nullptr);
}

/* ========================================================================== */

VkCommandBuffer	CommandBuffer::getBuffer() const noexcept {
	return _buffer;
}

CommandBuffer::operator VkCommandBuffer() const noexcept {
	return _buffer;
}

} // namespace graphics
} // namespace scop