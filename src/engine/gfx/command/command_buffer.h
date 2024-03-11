/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_buffer.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 20:01:10 by etran             #+#    #+#             */
/*   Updated: 2024/03/11 14:03:19 by etran            ###   ########.fr       */
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

    void    startRecording(VkCommandBufferUsageFlags flags = 0) override;
    void    stopRecording() override;
    void    awaitEndOfRecording(const Device& device) override;

    void    submitRecording(
        const std::vector<VkSemaphore> waitSemaphores,
        const std::vector<VkPipelineStageFlags> waitStages,
        const std::vector<VkSemaphore> signalSemaphore,
        const Fence& fence) override;

    void    reset() override;

    VkCommandBuffer getBuffer() const noexcept override;

protected:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    VkCommandBuffer     m_buffer;
    Fence               m_awaitFence;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    CommandBuffer() = default;

    CommandBuffer(CommandBuffer&& other) = delete;
    CommandBuffer(const CommandBuffer& other) = delete;
    CommandBuffer& operator=(CommandBuffer&& other) = delete;
    CommandBuffer& operator=(const CommandBuffer& other) = delete;

    /* ====================================================================== */

    virtual const VkCommandPool&    _getPool() const = 0;
    virtual const VkQueue&          _getQueue() const = 0;

}; // class CommandBuffer

} // namespace vox::gfx