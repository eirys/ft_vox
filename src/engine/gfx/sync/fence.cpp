/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fence.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 11:30:58 by etran             #+#    #+#             */
/*   Updated: 2024/03/11 13:48:45 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fence.h"
#include "device.h"

#include <stdexcept>

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void Fence::init(const Device& device, const VkFenceCreateFlags flags) {
    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = flags;

    if (vkCreateFence(device.getDevice(), &fenceInfo, nullptr, &m_fence) != VK_SUCCESS)
        throw std::runtime_error("failed to create fence");
}

void Fence::destroy(const Device& device) {
    vkDestroyFence(device.getDevice(), m_fence, nullptr);
}

/* ========================================================================== */

void Fence::await(const Device& device, const u64 timeout) const {
    vkWaitForFences(device.getDevice(), 1, &m_fence, VK_TRUE, timeout);
}

void Fence::reset(const Device& device) const {
    vkResetFences(device.getDevice(), 1, &m_fence);
}

/* ========================================================================== */

VkFence Fence::getFence() const noexcept {
    return m_fence;
}

} // namespace vox::gfx
