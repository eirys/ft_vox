/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:08:27 by etran             #+#    #+#             */
/*   Updated: 2024/03/18 12:19:13 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chunk.h"
#include "perlin_noise.h"

namespace game {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void Chunk::generate(
    const proc::PerlinNoise& noise,
    const u32 offsetX,
    const u32 offsetZ
) noexcept {
    for (u32 z = 0; z < CHUNK_SIZE; ++z) {
        for (u32 x = 0; x < CHUNK_SIZE; ++x) {
            const Height noiseValue = (Height)noise.noiseAt(x + offsetX, z + offsetZ);
            m_heights[z * CHUNK_SIZE + x] = noiseValue;
        }
    }
}

Chunk::Block& Chunk::operator[](const u32 index) noexcept {
    return m_blocks[index];
}

Chunk::Block Chunk::operator[](const u32 index) const noexcept {
    return m_blocks[index];
}

const std::array<Chunk::Height, CHUNK_AREA>& Chunk::getHeights() const {
    return m_heights;
}

} // namespace game