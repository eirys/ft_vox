/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vertex_buffer.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 13:34:05 by etran             #+#    #+#             */
/*   Updated: 2024/06/03 09:13:27 by etran            ###   ########.fr       */
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
    void    init(
        const Device& device,
        const game::GameState& gameState);
#else
    void    init(
        const Device& device,
        const ICommandBuffer* cmdBuffer,
        const game::GameState& gameState);
#endif
    void    destroy(const Device& device);

    void    update(
        const Device& device,
        const game::GameState& gameState);

    /* ====================================================================== */

    const Buffer&   getBuffer() const;
    u32             getInstancesCount() const noexcept;

    /* ====================================================================== */

    static void computeMaxVertexInstanceCount(const game::GameState& gameState);

private:
    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static u32  ms_maxVertexInstanceCount;

    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    Buffer  m_buffer;

    u32     m_instancesCount = 0;
    u32     m_visibleAABBsCount = 0;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

#if ENABLE_FRUSTUM_CULLING
    std::vector<VertexInstance> _computeVertexInstances(const game::GameState& gameState, u32& visibleAABBs) const;
#else
    std::vector<VertexInstance> _computeVertexInstances(const game::GameState& gameState) const;
#endif

}; // class VertexBuffer

} // namespace vox::gfx