/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics_command_buffer.h                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 22:27:11 by etran             #+#    #+#             */
/*   Updated: 2024/02/24 00:16:12 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "command_buffer.h"

namespace vox {
class Device;
} // namespace vox

namespace vox::gfx {

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
        m_pool(pool),
        m_queue(queue) {}

    ~GraphicsCommandBuffer() = default;

    GraphicsCommandBuffer() = delete;
    GraphicsCommandBuffer(GraphicsCommandBuffer&& other) = delete;
    GraphicsCommandBuffer(const GraphicsCommandBuffer& other) = delete;
    GraphicsCommandBuffer& operator=(GraphicsCommandBuffer&& other) = delete;
    GraphicsCommandBuffer& operator=(const GraphicsCommandBuffer& other) = delete;

    /* ====================================================================== */

    using super::init;
    using super::destroy;

    using super::startRecording;
    using super::reset;
    using super::getBuffer;

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    const VkCommandPool&    m_pool;
    const VkQueue&          m_queue;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    const VkCommandPool&    _getPool() const override { return m_pool; }
    const VkQueue&          _getQueue() const override { return m_queue; }

}; // class GraphicsCommandBuffer

} // namespace vox::gfx