/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icommand_buffer.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 10:28:53 by etran             #+#    #+#             */
/*   Updated: 2024/06/12 11:35:02 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace vox::gfx {

class Device;
class Fence;

enum class CommandBufferType {
    DRAW = VK_PIPELINE_BIND_POINT_GRAPHICS,
    COMPUTE = VK_PIPELINE_BIND_POINT_COMPUTE,
};

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

    virtual void    submitRecording(
        const std::vector<VkSemaphore> waitSemaphores,
        const std::vector<VkPipelineStageFlags> waitStages,
        const std::vector<VkSemaphore> signalSemaphore,
        const Fence& fence) const = 0;

    virtual void    bindPipeline(const VkPipeline& pipeline) const = 0;
    virtual void    bindDescriptorSets(
        const VkPipelineLayout& layout,
        const VkDescriptorSet* descriptorSets,
        const u32 setCount) const = 0;

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