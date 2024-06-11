/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:08:27 by etran             #+#    #+#             */
/*   Updated: 2024/06/11 23:49:40 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chunk.h"
#include "perlin_noise.h"
#include "voxmap.h"

#include "debug.h"

namespace proc {
using VoronoiDiagram = VoxMap;
}

namespace game {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void Chunk::generate(
    const proc::PerlinNoise& terrainNoise,
    // const proc::PerlinNoise& moistureNoise,
    const proc::VoronoiDiagram& biomeMap,
    const u32 offsetX,
    const u32 offsetY,
    const u32 offsetZ
) noexcept {
    m_blocks.resize(CHUNK_VOLUME);

    constexpr math::Vect3 HALF_CHUNK = math::Vect3(CHUNK_SIZE / 2.0f);

    m_boundingBox = vox::gfx::BoundingBox(
        math::Vect3(offsetX, offsetY, offsetZ) * CHUNK_SIZE + HALF_CHUNK,
        HALF_CHUNK);
    m_position = { offsetX, offsetY, offsetZ };

    // Only generate at height 0
    if (offsetY == 0) {
        for (u32 z = 0; z < CHUNK_SIZE; ++z) {
            for (u32 x = 0; x < CHUNK_SIZE; ++x) {
                const u32 blockX = x + (offsetX * CHUNK_SIZE);
                const u32 blockZ = z + (offsetZ * CHUNK_SIZE);

                const f32 moisture = 0.0f; // moistureNoise.noiseAt(blockX, blockZ);
                const Biome biome = _getBiome(biomeMap.getValue((f32)blockX, (f32)blockZ), moisture);
                const u8 terrainHeight = _generateHeight(terrainNoise, biome, blockX, blockZ);

                MaterialType material = MaterialType::Dirt;

                for (u32 y = 0; y < terrainHeight; ++y) {
                    material = _getMaterial(biome, y);
                    m_blocks[(y * CHUNK_AREA) + (z * CHUNK_SIZE) + x] = Block(material, biome);
                }

                if (material == MaterialType::Dirt)
                    m_blocks[(terrainHeight * CHUNK_AREA) + (z * CHUNK_SIZE) + x] = MaterialType::Grass;
                else
                    m_blocks[(terrainHeight * CHUNK_AREA) + (z * CHUNK_SIZE) + x] = Block(material, biome);
            }
        }
    }
}

Block& Chunk::operator[](const u32 index) noexcept {
    return m_blocks[index];
}

const Block& Chunk::operator[](const u32 index) const noexcept {
    return m_blocks[index];
}

Block& Chunk::getBlock(const u32 x, const u32 y, const u32 z) noexcept {
    return m_blocks[(y * CHUNK_AREA) + (z * CHUNK_SIZE) + x];
}

const Block& Chunk::getBlock(const u32 x, const u32 y, const u32 z) const noexcept {
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

const vox::gfx::BoundingBox& Chunk::getBoundingBox() const noexcept {
    return m_boundingBox;
}

/**
 * @brief Cache the chunk.
*/
void Chunk::cache() const {
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

Biome Chunk::_getBiome(const f32 cellValue, const f32 moisture) const noexcept {
    if (cellValue < 0.4f && moisture > 0.0f) return Biome::Oceans;
    else if (cellValue < 0.5f) return Biome::Plains;
    else if (cellValue < 0.6f) return Biome::Desert;
    else if (cellValue < 0.9f) return Biome::RockMountains;
    return Biome::SnowMountains;
}

u8 Chunk::_generateHeight(
    const proc::PerlinNoise& terrainNoise,
    const Biome biome,
    const u32 x,
    const u32 z
) const noexcept {
    const u8 noiseValue = (u8)terrainNoise.noiseAt(x, z);

    switch (biome) {
        case Biome::Plains:
        case Biome::Desert:
            return noiseValue / 2;

        //TODO: Implement height generation for these biomes
        case Biome::SnowMountains:
        case Biome::RockMountains:
            return noiseValue;

        case Biome::Oceans:
        default:
            return 1;
    }
    return 0;
}

MaterialType Chunk::_getMaterial(const Biome biome, const u8 height) const noexcept {
    switch (biome) {
        case Biome::Plains:
            if (height == 0)                return MaterialType::Stone; // Water
            else if (height < 2)            return MaterialType::Stone;
            else if (height < 5)           return MaterialType::Dirt;
            else if (height < 16)           return MaterialType::Grass;

        case Biome::Desert:
            if (height == 0)                return MaterialType::Stone; // Water
            else                            return MaterialType::Sand;

        case Biome::SnowMountains:
        case Biome::RockMountains:
        case Biome::Oceans:
        default:
            return MaterialType::Stone;
    }

    // if (height == 0 && moistureValue > 0.0f) {
    //     return MaterialType::Water;
    // } else if (height < 5 && moistureValue > 0.0f) {
    //     return MaterialType::Sand;
    // } else if (height < 10) {
    //     return MaterialType::Stone;
    // } else if (height < 15) {
    //     return MaterialType::Dirt;
    // } else if (height < 20) {
    //     return MaterialType::Stone;
    // }
    // return MaterialType::Snow;
}

} // namespace game