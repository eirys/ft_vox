/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:29:06 by etran             #+#    #+#             */
/*   Updated: 2024/03/18 10:59:10 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <array>

#include "types.h"
#include "game_decl.h"

namespace proc {
class PerlinNoise;
}

namespace game {

class Chunk final {
public:
    /* ====================================================================== */
    /*                                TYPEDEFS                                */
    /* ====================================================================== */

    using Block = u8;
    using Height = u8;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void    generate(const proc::PerlinNoise& noise, const u32 offsetX, const u32 offsetZ) noexcept;

    Block&  operator[](const u32 index) noexcept;
    Block   operator[](const u32 index) const noexcept;

    const std::array<Height, CHUNK_AREA>&    getHeights() const;

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    std::array<Block, CHUNK_VOLUME> m_blocks = {};
    std::array<Height, CHUNK_AREA>     m_heights = {};

}; // class Chunk

} // namespace game