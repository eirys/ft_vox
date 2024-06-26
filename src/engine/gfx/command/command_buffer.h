/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_buffer.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 20:01:10 by etran             #+#    #+#             */
/*   Updated: 2024/06/14 19:28:30 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "types.h"
# include "icommand_buffer.h"
# include "fence.h"

namespace vox::gfx {

class CommandBuffer: public ICommandBuffer {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    virtual ~CommandBuffer() = default;

    /* ====================================================================== */

    void    init(const Device& device, const VkCommandBufferLevel level) override;
    void    destroy(const Device& device) override;

    void    reset() const override;
    void    startRecording(VkCommandBufferUsageFlags flags = 0) const override;
    void    stopRecording() const override;
    void    awaitEndOfRecording(const Device& device) const override;

    void    submitRecording(
        const std::vector<VkSemaphore>& waitSemaphores,
        const std::vector<VkPipelineStageFlags>& waitStages,
        const std::vector<VkSemaphore>& signalSemaphore,
        const VkFence fence = VK_NULL_HANDLE) const override;

    void    bindPipeline(const VkPipeline& pipeline) const override;
    void    bindDescriptorSets(const PipelineLayout& pipelineLayout) const override;

    VkCommandBuffer getBuffer() const noexcept override;

protected:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    VkCommandBuffer     m_buffer = VK_NULL_HANDLE;
    Fence               m_awaitFence;
    CommandBufferType   m_type;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    CommandBuffer(const CommandBufferType type) noexcept;

    CommandBuffer() = delete;
    CommandBuffer(CommandBuffer&& other) = delete;
    CommandBuffer(const CommandBuffer& other) = delete;
    CommandBuffer& operator=(CommandBuffer&& other) = delete;
    CommandBuffer& operator=(const CommandBuffer& other) = delete;

    /* ====================================================================== */

    virtual const VkCommandPool&    _getPool() const = 0;
    virtual const VkQueue&          _getQueue() const = 0;

}; // class CommandBuffer

} // namespace vox::gfx