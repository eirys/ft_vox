/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_pool.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 12:47:19 by etran             #+#    #+#             */
/*   Updated: 2024/01/31 12:19:14 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_pool.h"
#include "device.h"
#include "command_buffer.h"

#include <cassert> // assert
#include <stdexcept> // std::runtime_error

namespace scop::gfx {

VkCommandPool	CommandPool::_draw_pool = VK_NULL_HANDLE;
VkCommandPool	CommandPool::_compute_pool = VK_NULL_HANDLE;

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

/**
 * @brief Creates both graphics and compute command pools.
*/
void	CommandPool::init(scop::core::Device& device) {
	scop::core::QueueFamilyIndices	queue_family_indices = device.findQueueFamilies();

	VkCommandPoolCreateInfo	pool_info{};
	pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	{ // Graphics pool
		pool_info.queueFamilyIndex = queue_family_indices.graphics_family.value();
		if (vkCreateCommandPool(device.getLogicalDevice(), &pool_info, nullptr, &_draw_pool) != VK_SUCCESS) {
			throw std::runtime_error("failed to create graphics command pool");
		}
	}
	{ // Compute pool
		pool_info.queueFamilyIndex = queue_family_indices.compute_family.value();

		if (vkCreateCommandPool(device.getLogicalDevice(), &pool_info, nullptr, &_compute_pool) != VK_SUCCESS) {
			throw std::runtime_error("failed to create compute command pool");
		}
	}
}

void	CommandPool::destroy(scop::core::Device& device) {
	vkDestroyCommandPool(device.getLogicalDevice(), _draw_pool, nullptr);
	vkDestroyCommandPool(device.getLogicalDevice(), _compute_pool, nullptr);
}

CommandBuffer	CommandPool::createCommandBuffer(
	scop::core::Device& device,
	CommandBufferType type)
{
	CommandBuffer	command_buffer(type);
	command_buffer.init(device);

	return command_buffer;
}

void	CommandPool::destroyBuffer(scop::core::Device& device, CommandBuffer& buffer) {
	buffer.destroy(device);
}

/* ========================================================================== */

VkCommandPool	CommandPool::getDrawPool() noexcept {
	return _draw_pool;
}

VkCommandPool	CommandPool::getComputePool() noexcept {
	return _compute_pool;
}

} // namespace scop::gfx