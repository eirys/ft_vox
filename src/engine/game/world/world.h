/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:24:42 by etran             #+#    #+#             */
/*   Updated: 2024/03/15 17:51:11 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "chunk.h"

namespace game {

class World final {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void init(const u32 seed);

    const std::array<Chunk, WORLD_SIZE>&    getChunks() const noexcept;
    std::array<Chunk, WORLD_SIZE>&          getChunks() noexcept;

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    std::array<Chunk, WORLD_SIZE>   m_chunks;

}; // class World

} // namespace game