/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vertex_buffer.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 13:34:05 by etran             #+#    #+#             */
/*   Updated: 2024/08/15 17:34:59 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <array>
#include <vector>
#include <stack>

#include "buffer.h"
#include "vertex.h"

namespace game {
class GameState;
class Chunk;
}

namespace vox::gfx {

class VertexBuffer final {
public:
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

    static void     bind(const ICommandBuffer* cmdBuffer);

    /* ====================================================================== */

    static const Buffer&   getBuffer() noexcept;
    static u32             getInstancesCount() noexcept;

private:
    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static Buffer   ms_buffer;
    static u32      ms_instancesCount;

}; // class VertexBuffer

} // namespace vox::gfx