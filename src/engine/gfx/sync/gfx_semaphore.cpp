/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gfx_semaphore.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 10:54:43 by etran             #+#    #+#             */
/*   Updated: 2024/03/02 12:32:56 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gfx_semaphore.h"
#include "device.h"

#include <stdexcept>

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void GfxSemaphore::init(const Device& device) {
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    if (vkCreateSemaphore(device.getDevice(), &semaphoreInfo, nullptr, &m_semaphore) != VK_SUCCESS)
        throw std::runtime_error("failed to create semaphore");
}

void GfxSemaphore::destroy(const Device& device) {
    vkDestroySemaphore(device.getDevice(), m_semaphore, nullptr);
}

/* ========================================================================== */

/**
 * @brief @brief Signals the semaphore, locking the queue until the semaphore is signaled.
*/
void GfxSemaphore::signal(const VkQueue queue) {
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &m_semaphore;

    if (vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
        throw std::runtime_error("failed to signal semaphore");
}

/* ========================================================================== */

VkSemaphore GfxSemaphore::getSemaphore() const noexcept {
    return m_semaphore;
}

} // namespace vox::gfx
