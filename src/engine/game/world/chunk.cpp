/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:08:27 by etran             #+#    #+#             */
/*   Updated: 2024/03/15 23:15:29 by etran            ###   ########.fr       */
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
            const f32 noiseValue = noise.noiseAt(x + offsetX, z + offsetZ) * CHUNK_HEIGHT;
            const u32 terrainHeight = (u32)std::floor(noiseValue);

            m_heights[z * CHUNK_SIZE + x] = terrainHeight;

            // // for (u32 y = 0; y < terrainHeight; ++y)
            // //     m_blocks[z * (CHUNK_AREA) + (y * CHUNK_SIZE + x)] = 1;
        }
    }
}

u8& Chunk::operator[](const u32 index) noexcept {
    return m_blocks[index];
}

u8 Chunk::operator[](const u32 index) const noexcept {
    return m_blocks[index];
}

const std::array<u32, CHUNK_AREA>& Chunk::getHeights() const {
    return m_heights;
}

} // namespace game