/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:08:27 by etran             #+#    #+#             */
/*   Updated: 2024/05/29 11:13:41 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chunk.h"
#include "perlin_noise.h"

#include "debug.h"
namespace game {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void Chunk::generate(
    const proc::PerlinNoise& noise,
    const u32 offsetX,
    const u32 offsetY,
    const u32 offsetZ
) noexcept {
    m_position = { offsetX, offsetY, offsetZ };

    // Only generate at height 0
    if (offsetY == 0) {
        for (u32 z = 0; z < CHUNK_SIZE; ++z) {
            for (u32 x = 0; x < CHUNK_SIZE; ++x) {
                const u8 terrainHeight = (u8)noise.noiseAt(x + (offsetX * CHUNK_SIZE), z + (offsetZ * CHUNK_SIZE));
                for (u32 y = 0; y < terrainHeight; ++y)
                    m_blocks[(y * CHUNK_AREA) + (z * CHUNK_SIZE) + x] = 1;
            }
        }
    }
}

Block& Chunk::operator[](const u32 index) noexcept {
    return m_blocks[index];
}

Block Chunk::operator[](const u32 index) const noexcept {
    return m_blocks[index];
}

Block& Chunk::getBlock(const u32 x, const u32 y, const u32 z) noexcept {
    return m_blocks[(y * CHUNK_AREA) + (z * CHUNK_SIZE) + x];
}

Block Chunk::getBlock(const u32 x, const u32 y, const u32 z) const noexcept {
    return m_blocks[(y * CHUNK_AREA) + (z * CHUNK_SIZE) + x];
}

const Chunk::BlockArray& Chunk::getBlocks() const {
    return m_blocks;
}

/**
 * @brief Returns packed chunk position. cf. chart.md
 */
u16 Chunk::getId() const {
    constexpr u32 sizeOfX = 5;
    constexpr u32 sizeOfZ = 5;
    constexpr u32 sizeOfY = 4;

    static_assert((sizeOfX + sizeOfY + sizeOfZ) < 16, "Update id size.");

    return (m_position.m_x << (sizeOfZ + sizeOfY)) | (m_position.m_y << sizeOfX) | m_position.m_z;
}

/**
 * @brief Cache the chunk.
*/
void Chunk::cache() const {
}

} // namespace game