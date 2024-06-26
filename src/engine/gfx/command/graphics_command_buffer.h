/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics_command_buffer.h                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 22:27:11 by etran             #+#    #+#             */
/*   Updated: 2024/06/12 11:27:25 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "command_buffer.h"

namespace vox::gfx {

class Device;

class GraphicsCommandBuffer final: public CommandBuffer {
public:
    /* ====================================================================== */
    /*                                TYPEDEFS                                */
    /* ====================================================================== */

    using super = CommandBuffer;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    GraphicsCommandBuffer(const VkCommandPool& pool, const VkQueue& queue):
        super(CommandBufferType::DRAW),
        m_pool(pool),
        m_queue(queue) {}

    ~GraphicsCommandBuffer() = default;

    GraphicsCommandBuffer() = delete;
    GraphicsCommandBuffer(GraphicsCommandBuffer&& other) = delete;
    GraphicsCommandBuffer(const GraphicsCommandBuffer& other) = delete;
    GraphicsCommandBuffer& operator=(GraphicsCommandBuffer&& other) = delete;
    GraphicsCommandBuffer& operator=(const GraphicsCommandBuffer& other) = delete;

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

}; // class GraphicsCommandBuffer

} // namespace vox::gfx