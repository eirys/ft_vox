/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_buffer.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 20:01:10 by etran             #+#    #+#             */
/*   Updated: 2024/02/24 00:16:19 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "icommand_buffer.h"
# include "types.h"

namespace vox::gfx {

class CommandBuffer: public ICommandBuffer {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    virtual ~CommandBuffer() = default;

    /* ====================================================================== */

    void    init(const vox::Device& device, const VkCommandBufferLevel level) override;
    void    destroy(const vox::Device& device) override;

    void    startRecording(VkCommandBufferUsageFlags flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT) override;
    void    stopRecording(const vox::Device& device, bool await = false) override;
    void    reset() override;

    VkCommandBuffer getBuffer() const noexcept override;

protected:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    VkCommandBuffer     m_buffer;

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