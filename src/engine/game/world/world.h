/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:24:42 by etran             #+#    #+#             */
/*   Updated: 2024/05/28 14:32:58 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "chunk.h"

namespace game {

/**
 * @brief The World class represents the game world.
 * Gives information on chunks and their data.
 */
class World final {
public:
    /* ====================================================================== */
    /*                                TYPEDEFS                                */
    /* ====================================================================== */

    using ChunkArray = std::array<Chunk, RENDER_AREA * RENDER_HEIGHT>;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void init(const u32 seed);

    const ChunkArray&   getChunks() const noexcept;
    ChunkArray&         getChunks() noexcept;

    Chunk&              getChunk(const u32 x, const u32 y, const u32 z) noexcept;
    const Chunk&        getChunk(const u32 x, const u32 y, const u32 z) const noexcept;

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    ChunkArray  m_chunks;

}; // class World

} // namespace game