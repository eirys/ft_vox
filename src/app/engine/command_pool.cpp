/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_pool.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 12:47:19 by etran             #+#    #+#             */
/*   Updated: 2023/07/04 09:42:49 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_pool.h"
#include "engine.h"
#include "device.h"

namespace scop {
namespace graphics {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void	CommandPool::init(Device& device) {
	Device::QueueFamilyIndices	queue_family_indices =
		device.findQueueFamilies();

	VkCommandPoolCreateInfo	pool_info{};
	pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	pool_info.queueFamilyIndex = queue_family_indices.graphics_family.value();

	if (vkCreateCommandPool(device.getLogicalDevice(), &pool_info, nullptr, &_pool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command pool");
	}
}

void	CommandPool::destroy(Device& device) {
	vkDestroyCommandPool(device.getLogicalDevice(), _pool, nullptr);
}

/* ========================================================================== */

VkCommandPool	CommandPool::getPool() const noexcept {
	return _pool;
}

CommandPool::operator VkCommandPool() const noexcept {
	return _pool;
}

} // namespace graphics
} // namespace scop