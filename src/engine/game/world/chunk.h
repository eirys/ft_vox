/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:29:06 by etran             #+#    #+#             */
/*   Updated: 2024/05/28 14:22:45 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <array>

#include "types.h"
#include "game_decl.h"
#include "block.h"

namespace proc {
class PerlinNoise;
}

namespace game {

class Chunk final {
public:
    /* ====================================================================== */
    /*                                TYPEDEFS                                */
    /* ====================================================================== */

    using BlockArray = std::array<Block, CHUNK_VOLUME>;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    Chunk() = default;
    ~Chunk() = default;
    Chunk(Chunk&& other) = default;
    Chunk& operator=(Chunk&& other) = default;

    Chunk(const Chunk& other) = delete;
    Chunk& operator=(const Chunk& other) = delete;

    /* ====================================================================== */

    void    generate(
        const proc::PerlinNoise& noise,
        const u32 offsetX,
        const u32 offsetY,
        const u32 offsetZ) noexcept;

    Block&  operator[](const u32 index) noexcept;
    Block   operator[](const u32 index) const noexcept;
    Block&  getBlock(const u32 x, const u32 y, const u32 z) noexcept;
    Block   getBlock(const u32 x, const u32 y, const u32 z) const noexcept;

    const BlockArray&   getBlocks() const;
    u16                 getId() const;

    /* ====================================================================== */

    void                cache() const;

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    BlockArray      m_blocks = {};
    struct {
        u32 m_x = 0;
        u32 m_y = 0;
        u32 m_z = 0;
    }               m_position;
    bool            m_updated = false;

}; // class Chunk

} // namespace game