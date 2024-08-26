/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:08:27 by etran             #+#    #+#             */
/*   Updated: 2024/08/22 17:22:03 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chunk.h"
#include "voxmap.h"
#include "world.h"

#include "debug.h"

namespace game {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void Chunk::generate(
    const proc::VoxMap& terrainNoise,
    // const proc::PerlinNoise& moistureNoise,
    const proc::VoxMap& biomeMap,
    const u32 offsetX,
    const u32 offsetZ
) {
}

void Chunk::generateTerrain(
    const proc::VoxMap& terrainNoise,
    // const proc::PerlinNoise& moistureNoise,
    const proc::VoxMap& biomeMap,
    const u32 offsetX,
    const u32 offsetZ
) {
    constexpr math::Vect3 HALF_CHUNK = math::Vect3(SIZE / 2.0f);

    m_chunkGfx.m_boundingBox = vox::gfx::BoundingBox(
        math::Vect3(offsetX, 0.0f, offsetZ) * SIZE + HALF_CHUNK,
        HALF_CHUNK);
    m_position = { offsetX, offsetZ };

    m_blocks.resize(VOLUME);

    for (u32 z = 0; z < SIZE; ++z) {
        for (u32 x = 0; x < SIZE; ++x) {
            const u32 blockX = x + (offsetX * SIZE);
            const u32 blockZ = z + (offsetZ * SIZE);

            const f32 moisture = 0.0f; // moistureNoise.noiseAt(blockX, blockZ);
            const Biome biome = _getBiome(biomeMap.getValue((f32)blockX, (f32)blockZ), moisture);
            const u8 terrainHeight = _getHeight(terrainNoise, biome, blockX, blockZ);

            MaterialType material = MaterialType::Dirt;

            for (u32 y = 0; y < terrainHeight; ++y) {
                material = _getMaterial(biome, y);
                m_blocks[(y * AREA) + (z * SIZE) + x] = Block(material, biome);
            }

            if (material == MaterialType::Dirt)
                m_blocks[(terrainHeight * AREA) + (z * SIZE) + x] = Block(MaterialType::Grass, biome);
            else
                m_blocks[(terrainHeight * AREA) + (z * SIZE) + x] = Block(material, biome);
        }
    }
}

static
bool _isExposed(const game::Chunk* neighbor, const u32 checkedX, const u32 checkedY, const u32 checkedZ) {
    return neighbor == nullptr ||
    (neighbor &&
    neighbor->getBlock(checkedX, checkedY, checkedZ).isVoid());
}

void Chunk::generateInstances(std::vector<vox::gfx::VertexInstance>& instances, const std::vector<const Chunk*>& neighbors) {
    constexpr u32 HEIGHT_LIMIT = HEIGHT - 1;
    constexpr u32 SIZE_LIMIT = SIZE - 1;

    const u32 oldInstanceCount = instances.size();

    for (u32 z = 0; z < SIZE; ++z) {
        for (u32 x = 0; x < SIZE; ++x) {
            for (u32 y = 0; y < HEIGHT; ++y) {
                const auto& block = getBlock(x, y, z);

                if (block.isVoid())
                    continue;

                const u16 blockId = (x << 12) | (y << 4) | z;
                const u16 chunkId = getId();

                // Check if block face is visible
                if (y == HEIGHT_LIMIT || getBlock(x, y + 1, z).isVoid())
                    instances.emplace_back(game::BlockFace::Top, blockId, chunkId);
                if (y == 0 || getBlock(x, y - 1, z).isVoid())
                    instances.emplace_back(game::BlockFace::Bottom, blockId, chunkId);

                if ((x != SIZE_LIMIT && getBlock(x + 1, y, z).isVoid()) || (x == SIZE_LIMIT && _isExposed(neighbors[ChunkNeighbor::Right], 0, y, z)))
                    instances.emplace_back(game::BlockFace::Right, blockId, chunkId);
                if ((x != 0 && getBlock(x - 1, y, z).isVoid()) || (x == 0 && _isExposed(neighbors[ChunkNeighbor::Left], SIZE_LIMIT, y, z)))
                    instances.emplace_back(game::BlockFace::Left, blockId, chunkId);
                if ((z != SIZE_LIMIT && getBlock(x, y, z + 1).isVoid()) || (z == SIZE_LIMIT && _isExposed(neighbors[ChunkNeighbor::Front], x, y, 0)))
                    instances.emplace_back(game::BlockFace::Front, blockId, chunkId);
                if ((z != 0 && getBlock(x, y, z - 1).isVoid()) || (z == 0 && _isExposed(neighbors[ChunkNeighbor::Back], x, y, SIZE_LIMIT)))
                    instances.emplace_back(game::BlockFace::Back, blockId, chunkId);
            }
        }
    }

    m_chunkGfx.m_instanceCount = instances.size() - oldInstanceCount;
}

void Chunk::clearBlocks() {
    if (!SHOULD_STORE_BLOCKS) {
        m_blocks.clear();
        m_blocks.shrink_to_fit();
    }
}

Block& Chunk::operator[](const u32 index) noexcept {
    return m_blocks[index];
}

const Block& Chunk::operator[](const u32 index) const noexcept {
    return m_blocks[index];
}

Block& Chunk::getBlock(const u32 x, const u32 y, const u32 z) noexcept {
    return m_blocks[(y * AREA) + (z * SIZE) + x];
}

const Block& Chunk::getBlock(const u32 x, const u32 y, const u32 z) const noexcept {
    return m_blocks[(y * AREA) + (z * SIZE) + x];
}

const std::vector<Block>& Chunk::getBlocks() const {
    return m_blocks;
}

/**
 * @brief Returns packed chunk position. cf. chart.md
 */
u16 Chunk::getId() const {
    return m_position.x + m_position.z * World::SIDE;
}

u32 Chunk::getInstanceCount() const noexcept {
    return m_chunkGfx.m_instanceCount;
}

bool Chunk::isVisible(const vox::gfx::BoundingFrustum& frustum) const noexcept {
    return m_chunkGfx.m_boundingBox.isVisible(frustum);
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
    else if (cellValue < 0.05f) return Biome::Plains;
    else if (cellValue < 0.6f) return Biome::Desert;
    else if (cellValue < 0.9f) return Biome::RockMountains;
    return Biome::Plains;
}

u8 Chunk::_getHeight(
    const proc::VoxMap& terrainNoise,
    const Biome biome,
    const u32 x,
    const u32 z
) const noexcept {
    const u8 noiseValue = (u8)terrainNoise.getValue(x, z);

    switch (biome) {
        case Biome::Plains:
        case Biome::Desert:
            return noiseValue / 2;

        //TODO: Implement height generation for these biomes
        // case Biome::SnowMountains:
        // case Biome::RockMountains:
        //     return noiseValue;
        case Biome::Oceans:
            return 1;

        default:
            return noiseValue;
    }
    return 0;
}

MaterialType Chunk::_getMaterial(const Biome biome, const u8 height) const noexcept {
    switch (biome) {
        case Biome::Plains:
            if (height == 0)                return MaterialType::Water;
            else if (height < 2)            return MaterialType::Stone;
            else if (height < 5)            return MaterialType::Dirt;
            else                            return MaterialType::Grass;

        case Biome::Desert:
            if (height == 0)                return MaterialType::Water;
            else                            return MaterialType::Sand;

        case Biome::SnowMountains:
        case Biome::RockMountains:
        case Biome::Oceans:
        default:
            return MaterialType::Stone;
    }
}

} // namespace game