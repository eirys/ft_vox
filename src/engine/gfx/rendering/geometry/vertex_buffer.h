/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vertex_buffer.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 13:34:05 by etran             #+#    #+#             */
/*   Updated: 2024/05/30 22:44:55 by etran            ###   ########.fr       */
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

    void    init(
        const Device& device,
        const ICommandBuffer* cmdBuffer,
        const game::GameState& gameState);
    void    destroy(const Device& device);

    /* ====================================================================== */

    const Buffer&   getBuffer() const;
    u32             getInstancesCount() const noexcept;

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    Buffer  m_buffer;
    u32     m_instancesCount = 0;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void                        _evaluateChunk(const game::Chunk& chunk, std::vector<VertexInstance>& instances) const;
    std::vector<VertexInstance> _computeVerticeInstances(const game::GameState& gameState) const;

}; // class VertexBuffer

} // namespace vox::gfx