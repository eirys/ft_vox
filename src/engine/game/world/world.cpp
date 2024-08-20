/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:51:38 by etran             #+#    #+#             */
/*   Updated: 2024/08/20 13:43:22 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "world.h"
#include "perlin_noise.h"
#include "voxmap.h"

#include "debug.h"

namespace game {

World::Settings World::ms_settings;

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void World::init() {
    m_chunks.resize(DIMENSION);

    proc::VoxMap perlin;
    proc::VoxMap voronoi;

    if (voronoi.load("assets/maps/biomes.voxmap") == false ||
        perlin.load("assets/maps/height.voxmap") == false) {
        throw std::runtime_error("Failed to load biome map.");
    }

    perlin.setScale(15.0f);

    for (u32 z = 0; z < SIZE; ++z)
        for (u32 x = 0; x < SIZE; ++x)
            m_chunks[(z * SIZE) + x].generateTerrain(perlin, voronoi, x, z);

    _generateInstances();

    m_origin = WORLD_ORIGIN;
    m_origin.y = perlin.getValue(m_origin.x, m_origin.z);

    LINFO("World initialized.");
}

/* ========================================================================== */

const std::vector<Chunk>& World::getChunks() const noexcept {
    return m_chunks;
}

std::vector<Chunk>& World::getChunks() noexcept {
    return m_chunks;
}

Chunk& World::getChunk(const u32 x, const u32 z) noexcept {
    return m_chunks[(z * SIZE) + x];
}

const Chunk& World::getChunk(const u32 x, const u32 z) const noexcept {
    return m_chunks[(z * SIZE) + x];
}

const math::Vect3& World::getOrigin() const noexcept {
    return m_origin;
}

const std::vector<vox::gfx::VertexInstance>& World::getInstances() const noexcept {
    return m_worldGfx.m_instances;
}

std::vector<u8> World::getBlockRaw() const {
    std::vector<u8> raw;
    raw.reserve(ms_settings.rendering.renderDistance * Chunk::VOLUME);

    for (const Chunk& chunk: m_chunks) {
        for (u32 z = 0; z < Chunk::SIZE; ++z) {
            for (u32 x = 0; x < Chunk::SIZE; ++x) {
                for (u32 y = 0; y < Chunk::HEIGHT; ++y) {
                    const auto& block = chunk.getBlock(x, y, z);
                    raw.emplace_back((u8)block.getMaterial());
                }
            }
        }
    }
    return raw;
}

const World::Settings& World::getSettings() noexcept {
    return ms_settings;
}
/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

void World::_generateInstances() {
    using ChunkNeighbor = game::ChunkNeighbor;

    std::vector<const game::Chunk*>  neighbors = {nullptr, nullptr, nullptr, nullptr};

    for (u32 z = 0; z < ms_settings.rendering.renderDistance; ++z) {
        for (u32 x = 0; x < ms_settings.rendering.renderDistance; ++x) {
            neighbors = {nullptr, nullptr, nullptr, nullptr};

            auto& chunk = getChunk(x, z);

            if (x > 0)
                neighbors[ChunkNeighbor::Left] = &getChunk(x - 1, z);
            if (x < ms_settings.rendering.renderDistance - 1)
                neighbors[ChunkNeighbor::Right] = &getChunk(x + 1, z);
            if (z > 0)
                neighbors[ChunkNeighbor::Back] = &getChunk(x, z - 1);
            if (z < ms_settings.rendering.renderDistance - 1)
                neighbors[ChunkNeighbor::Front] = &getChunk(x, z + 1);

            chunk.generateInstances(m_worldGfx.m_instances, neighbors);
        }
    }
}

void World::ignoreData() {
    for (auto& chunk: m_chunks) {
        chunk.clearBlocks();
    }
}

} // namespace game