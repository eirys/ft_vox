/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:24:42 by etran             #+#    #+#             */
/*   Updated: 2024/05/17 16:10:00 by etran            ###   ########.fr       */
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