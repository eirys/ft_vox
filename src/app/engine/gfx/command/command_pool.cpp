/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_pool.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 09:54:13 by etran             #+#    #+#             */
/*   Updated: 2024/02/28 15:49:03 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_pool.h"
#include "device.h"
#include "core.h"
#include "graphics_command_buffer.h"
#include "compute_command_buffer.h"

#include <stdexcept> // std::runtime_error

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void CommandPool::init(const Core& core, const Device& device) {
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    QueueFamilyIndices queueFamilyIndices = core.getQueueFamilyIndices();

    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
    if (vkCreateCommandPool(device.getDevice(), &poolInfo, nullptr, &m_drawPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics command pool");
    }

    poolInfo.queueFamilyIndex = queueFamilyIndices.computeFamily.value();
    if (vkCreateCommandPool(device.getDevice(), &poolInfo, nullptr, &m_computePool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create compute command pool");
    }
}

void CommandPool::destroy(const Device& device) {
    vkDestroyCommandPool(device.getDevice(), m_drawPool, nullptr);
    vkDestroyCommandPool(device.getDevice(), m_computePool, nullptr);
}

/* ========================================================================== */

ICommandBuffer* CommandPool::createCommandBuffer(
    const Device& device,
    CommandBufferType type,
    VkCommandBufferLevel level
) {
    ICommandBuffer* buffer = nullptr;

    switch (type) {
        case CommandBufferType::DRAW:       buffer = new GraphicsCommandBuffer(m_drawPool, device.getGraphicsQueue()); break;
        case CommandBufferType::COMPUTE:    buffer = new ComputeCommandBuffer(m_computePool, device.getComputeQueue()); break;
    }

    buffer->init(device, level);
    return buffer;
}

void CommandPool::destroyBuffer(const Device& device, ICommandBuffer* buffer) {
    buffer->destroy(device);
    delete buffer;
}

} // namespace vox::gfx