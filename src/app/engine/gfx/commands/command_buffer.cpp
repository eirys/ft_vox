/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_buffer.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 22:43:14 by etran             #+#    #+#             */
/*   Updated: 2024/02/24 00:20:50 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_buffer.h"
#include "device.h"
#include "graphics_command_buffer.h"
#include "compute_command_buffer.h"

#include <stdexcept>

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void CommandBuffer::init(
    const vox::Device& device,
    VkCommandBufferLevel level
) {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = _getPool();
    allocInfo.level = level;
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(device.getDevice(), &allocInfo, &m_buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers");
    }
}

void CommandBuffer::destroy(const vox::Device& device) {
    vkFreeCommandBuffers(device.getDevice(), _getPool(), 1, &m_buffer);
}

void CommandBuffer::startRecording(VkCommandBufferUsageFlags flags) {
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = flags;

    if (vkBeginCommandBuffer(m_buffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer");
    }
}

void CommandBuffer::reset() {
    vkResetCommandBuffer(m_buffer, 0);
}

VkCommandBuffer CommandBuffer::getBuffer() const noexcept {
    return m_buffer;
}

void CommandBuffer::stopRecording(const vox::Device& device, bool await) {
    if (vkEndCommandBuffer(m_buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer");
    }

    if (await) {
        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

        VkFence fence;
        if (vkCreateFence(device.getDevice(), &fenceInfo, nullptr, &fence) != VK_SUCCESS)
            throw std::runtime_error("failed to create fence");

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &m_buffer;

        if (vkQueueSubmit(_getQueue(), 1, &submitInfo, fence) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit command buffer to queue");
        }

        vkWaitForFences(device.getDevice(), 1, &fence, VK_TRUE, UINT64_MAX);
        vkDestroyFence(device.getDevice(), fence, nullptr);
    }
}

} // namespace vox::gfx