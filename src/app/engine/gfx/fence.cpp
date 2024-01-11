/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fence.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 21:41:06 by etran             #+#    #+#             */
/*   Updated: 2024/01/07 22:09:19 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fence.h"
#include "device.h"

#include <stdexcept> // std::runtime_error

namespace scop::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void	Fence::init(
	const scop::core::Device& device,
	VkFenceCreateFlagBits flags
) {
	VkFenceCreateInfo	fence_info = {};
	fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fence_info.flags = flags;

	if (vkCreateFence(device.getLogicalDevice(), &fence_info, nullptr, &_fence) != VK_SUCCESS)
		throw std::runtime_error("failed to create fence");
}

void	Fence::destroy(const scop::core::Device& device) {
	vkDestroyFence(device.getLogicalDevice(), _fence, nullptr);
}

void	Fence::wait(const scop::core::Device& device, uint64_t timeout) const {
	vkWaitForFences(device.getLogicalDevice(), 1, &_fence, VK_TRUE, timeout);
}

void	Fence::reset(const scop::core::Device& device) const {
	vkResetFences(device.getLogicalDevice(), 1, &_fence);
}

/* ========================================================================== */

VkFence	Fence::getFence() const noexcept {
	return _fence;
}

} // namespace scop::gfx