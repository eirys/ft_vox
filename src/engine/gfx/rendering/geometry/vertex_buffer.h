/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vertex_buffer.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 13:34:05 by etran             #+#    #+#             */
/*   Updated: 2024/09/17 15:28:03 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>

#include "buffer.h"

namespace vox::gfx {

class VertexBuffer final {
public:
    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static constexpr u32  BUFFER_COUNT = 2;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    VertexBuffer() = default;
    ~VertexBuffer() = default;

    VertexBuffer(VertexBuffer&& x) = delete;
    VertexBuffer(const VertexBuffer& x) = delete;
    VertexBuffer& operator=(VertexBuffer&& rhs) = delete;
    VertexBuffer& operator=(const VertexBuffer& rhs) = delete;

    /* ====================================================================== */

    static void     init(const Device& device, const ICommandBuffer* cmdBuffer);
    static void     destroy(const Device& device);

    static void     update(const Device& device, const ICommandBuffer* cmdBuffer);
    static void     bind(const ICommandBuffer* cmdBuffer);
    static void     changeBuffer() noexcept;

    /* ====================================================================== */

    static const Buffer&   getBuffer() noexcept { return ms_buffers[ms_currentBuffer]; }
    static u32             getInstancesCount() noexcept { return ms_buffers[ms_currentBuffer].getMetadata().m_size; }
    static u32             getCurrentBuffer() noexcept { return ms_currentBuffer; }

private:
    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static std::vector<Buffer>  ms_buffers;
    static u32                  ms_currentBuffer;

}; // class VertexBuffer

} // namespace vox::gfx