/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_pool.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 12:47:19 by etran             #+#    #+#             */
/*   Updated: 2023/11/16 21:57:36 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_pool.h"
#include "device.h"
#include "command_buffer.h"

#include <cassert> // assert
#include <stdexcept> // std::runtime_error

namespace scop::gfx {

VkCommandPool	CommandPool::_pool = VK_NULL_HANDLE;

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void	CommandPool::init(scop::core::Device& device) {
	scop::core::Device::QueueFamilyIndices	queue_family_indices =
		device.findQueueFamilies();

	VkCommandPoolCreateInfo	pool_info{};
	pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	pool_info.queueFamilyIndex = queue_family_indices.graphics_family.value();

	if (vkCreateCommandPool(device.getLogicalDevice(), &pool_info, nullptr, &_pool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command pool");
	}
}

void	CommandPool::destroy(scop::core::Device& device) {
	vkDestroyCommandPool(device.getLogicalDevice(), _pool, nullptr);
}

CommandBuffer	CommandPool::createBuffer(scop::core::Device& device) {
	assert(_pool != VK_NULL_HANDLE);

	CommandBuffer	command_buffer;
	command_buffer.init(device, _pool);

	return command_buffer;
}

void	CommandPool::destroyBuffer(scop::core::Device& device, CommandBuffer& buffer) {
	assert(_pool != VK_NULL_HANDLE);

	buffer.destroy(device, _pool);
}

/* ========================================================================== */

VkCommandPool	CommandPool::getPool() noexcept {
	return _pool;
}

CommandPool::operator VkCommandPool() const noexcept {
	return _pool;
}

} // namespace scop::gfx