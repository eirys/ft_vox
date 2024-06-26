/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compute_command_buffer.h                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 23:09:22 by etran             #+#    #+#             */
/*   Updated: 2024/06/12 11:28:01 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "command_buffer.h"

namespace vox::gfx {

class Device;

class ComputeCommandBuffer final: public CommandBuffer {
public:
    /* ====================================================================== */
    /*                                TYPEDEFS                                */
    /* ====================================================================== */

    using super = CommandBuffer;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    ComputeCommandBuffer(const VkCommandPool& pool, const VkQueue& queue):
        super(CommandBufferType::COMPUTE),
        m_pool(pool),
        m_queue(queue) {}
    ~ComputeCommandBuffer() = default;

    ComputeCommandBuffer() = delete;
    ComputeCommandBuffer(ComputeCommandBuffer&& other) = delete;
    ComputeCommandBuffer(const ComputeCommandBuffer& other) = delete;
    ComputeCommandBuffer& operator=(ComputeCommandBuffer&& other) = delete;
    ComputeCommandBuffer& operator=(const ComputeCommandBuffer& other) = delete;

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    const VkCommandPool&    m_pool;
    const VkQueue&          m_queue;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    inline
    const VkCommandPool&    _getPool() const override { return m_pool; }
    inline
    const VkQueue&          _getQueue() const override { return m_queue; }

}; // class ComputeCommandBuffer

} // namespace vox::gfx