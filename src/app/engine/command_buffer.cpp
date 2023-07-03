/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_buffer.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 12:47:19 by etran             #+#    #+#             */
/*   Updated: 2023/07/03 11:06:47 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_buffer.h"
#include "engine.h"
#include "device.h"

namespace scop {
namespace graphics {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void	CommandBuffer::initPool(Device& device) {
	createCommandPool(device);
}

void	CommandBuffer::initBuffer(Device& device) {
	createCommandBuffers(device);
}

void	CommandBuffer::destroy(Device& device) {
	vkDestroyCommandPool(device.getLogicalDevice(), vk_command_pool, nullptr);
}

/* ========================================================================== */

/**
 * @brief Initialize the command buffer
*/
void	CommandBuffer::reset() {
	vkResetCommandBuffer(command_buffers, 0);
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

/**
 * Command buffers memory handler
*/
void	CommandBuffer::createCommandPool(Device& device) {
	Device::QueueFamilyIndices	queue_family_indices =
		device.findQueueFamilies();

	VkCommandPoolCreateInfo	pool_info{};
	pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	pool_info.queueFamilyIndex = queue_family_indices.graphics_family.value();

	if (vkCreateCommandPool(device.getLogicalDevice(), &pool_info, nullptr, &vk_command_pool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command pool");
	}
}

/**
 * Command buffers handler
*/
void	CommandBuffer::createCommandBuffers(Device& device) {
	VkCommandBufferAllocateInfo	alloc_info{};
	alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	alloc_info.commandPool = vk_command_pool;
	alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	alloc_info.commandBufferCount = static_cast<uint32_t>(Engine::max_frames_in_flight);

	if (vkAllocateCommandBuffers(device.getLogicalDevice(), &alloc_info, &command_buffers) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers");
	}
}

} // namespace graphics
} // namespace scop