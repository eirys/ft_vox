/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vertex_buffer.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 13:34:05 by etran             #+#    #+#             */
/*   Updated: 2024/06/12 12:07:53 by etran            ###   ########.fr       */
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

#if ENABLE_FRUSTUM_CULLING
    static void     init(const Device& device, const game::GameState& gameState);
    static void     update(const Device& device, const game::GameState& gameState);
#else
    static void     init(const Device& device, const ICommandBuffer* cmdBuffer, const game::GameState& gameState);
#endif
    static void     destroy(const Device& device);

    static void     bind(const ICommandBuffer* cmdBuffer);

    /* ====================================================================== */

    static const Buffer&   getBuffer() noexcept;
    static u32             getInstancesCount() noexcept;

    /* ====================================================================== */

    static void computeMaxVertexInstanceCount(const game::GameState& gameState);

private:
    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static Buffer   ms_buffer;

    static u32      ms_instancesCount;
    static u32      ms_visibleAABBsCount;

    static u32      ms_maxVertexInstanceCount;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

#if ENABLE_FRUSTUM_CULLING
    static std::vector<VertexInstance> _computeVertexInstances(const game::GameState& gameState, u32& visibleAABBs);
#else
    static std::vector<VertexInstance> _computeVertexInstances(const game::GameState& gameState);
#endif

}; // class VertexBuffer

} // namespace vox::gfx