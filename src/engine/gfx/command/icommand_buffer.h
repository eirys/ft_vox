/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icommand_buffer.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 10:28:53 by etran             #+#    #+#             */
/*   Updated: 2024/03/21 00:37:11 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace vox::gfx {

class Device;
class Fence;
class RenderPass;
struct RecordInfo;

class ICommandBuffer {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    virtual ~ICommandBuffer() = default;

    /* ====================================================================== */

    virtual void    init(const Device& device, const VkCommandBufferLevel level) = 0;
    virtual void    destroy(const Device& device) = 0;

    virtual void    reset() const = 0;
    virtual void    startRecording(VkCommandBufferUsageFlags flags = 0) const = 0;
    virtual void    stopRecording() const = 0;
    virtual void    awaitEndOfRecording(const Device& device) const = 0;

    virtual void    beginRenderPass(RenderPass* renderPass, const RecordInfo& recordInfo) const = 0;
    virtual void    endRenderPass(RenderPass* renderPass) const = 0;

    virtual void    submitRecording(
        const std::vector<VkSemaphore> waitSemaphores,
        const std::vector<VkPipelineStageFlags> waitStages,
        const std::vector<VkSemaphore> signalSemaphore,
        const Fence& fence) = 0;

    virtual VkCommandBuffer getBuffer() const noexcept = 0;

protected:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    ICommandBuffer() = default;

    ICommandBuffer(ICommandBuffer&& other) = delete;
    ICommandBuffer(const ICommandBuffer& other) = delete;
    ICommandBuffer& operator=(ICommandBuffer&& other) = delete;
    ICommandBuffer& operator=(const ICommandBuffer& other) = delete;

}; // class ICommandBuffer

} // namespace vox::gfx