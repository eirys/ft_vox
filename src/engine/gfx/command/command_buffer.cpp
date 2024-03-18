/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_buffer.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 22:43:14 by etran             #+#    #+#             */
/*   Updated: 2024/03/18 15:54:47 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_buffer.h"
#include "device.h"
#include "graphics_command_buffer.h"
#include "compute_command_buffer.h"
#include "gfx_semaphore.h"
#include "debug.h"

#include <stdexcept>

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void CommandBuffer::init(
    const Device& device,
    VkCommandBufferLevel level
) {
    m_awaitFence.init(device);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = _getPool();
    allocInfo.level = level;
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(device.getDevice(), &allocInfo, &m_buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers");
    }
}

void CommandBuffer::destroy(const Device& device) {
    vkFreeCommandBuffers(device.getDevice(), _getPool(), 1, &m_buffer);
    m_awaitFence.destroy(device);
}

/* ========================================================================== */

void CommandBuffer::reset() const {
    vkResetCommandBuffer(m_buffer, 0);
}

void CommandBuffer::startRecording(VkCommandBufferUsageFlags flags) const {
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = flags;

    if (vkBeginCommandBuffer(m_buffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer");
    }
    LDEBUG("Started recording...");
}

void CommandBuffer::stopRecording() const {
    if (vkEndCommandBuffer(m_buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer");
    }
    LDEBUG("Stopped recording...");
}

void CommandBuffer::awaitEndOfRecording(const Device& device) const {
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_buffer;

    if (vkQueueSubmit(_getQueue(), 1, &submitInfo, m_awaitFence.getFence()) != VK_SUCCESS)
        throw std::runtime_error("failed to submit command buffer to queue");

    m_awaitFence.await(device);
    m_awaitFence.reset(device);
}

void CommandBuffer::submitRecording(
    const std::vector<VkSemaphore> waitSemaphores,
    const std::vector<VkPipelineStageFlags> waitStages,
    const std::vector<VkSemaphore> signalSemaphore,
    const Fence& fence
) {
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_buffer;
    submitInfo.pWaitDstStageMask = waitStages.data();
    submitInfo.pWaitSemaphores = waitSemaphores.data();
    submitInfo.waitSemaphoreCount = (u32)waitSemaphores.size();
    submitInfo.pSignalSemaphores = signalSemaphore.data();
    submitInfo.signalSemaphoreCount = (u32)signalSemaphore.size();

    if (vkQueueSubmit(_getQueue(), 1, &submitInfo, fence.getFence()) != VK_SUCCESS)
        throw std::runtime_error("failed to submit command buffer to queue");
}

/* ========================================================================== */

VkCommandBuffer CommandBuffer::getBuffer() const noexcept {
    return m_buffer;
}

} // namespace vox::gfx