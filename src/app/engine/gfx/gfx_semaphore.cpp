/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gfx_semaphore.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 21:30:07 by etran             #+#    #+#             */
/*   Updated: 2024/01/31 16:11:00 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gfx_semaphore.h"
#include "device.h"

#include <stdexcept> // std::runtime_error

namespace scop::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void	GfxSemaphore::init(scop::core::Device& device) {
	VkSemaphoreCreateInfo	semaphore_info = {};
	semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	if (vkCreateSemaphore(device.getLogicalDevice(), &semaphore_info, nullptr, &_semaphore) != VK_SUCCESS)
		throw std::runtime_error("failed to create semaphore");
}

void	GfxSemaphore::destroy(scop::core::Device& device) {
	vkDestroySemaphore(device.getLogicalDevice(), _semaphore, nullptr);
}

/* ========================================================================== */

void	GfxSemaphore::signal(VkQueue queue) {
	VkSubmitInfo	submit_info = {};
	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	submit_info.signalSemaphoreCount = 1;
	submit_info.pSignalSemaphores = &_semaphore;

	if (vkQueueSubmit(queue, 1, &submit_info, VK_NULL_HANDLE) != VK_SUCCESS)
		throw std::runtime_error("failed to signal semaphore");
}

/* ========================================================================== */

VkSemaphore	GfxSemaphore::getSemaphore() const noexcept {
	return _semaphore;
}

} // namespace scop::gfx