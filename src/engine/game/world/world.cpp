/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:51:38 by etran             #+#    #+#             */
/*   Updated: 2024/09/17 16:17:15 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "world.h"
#include "voxmap.h"
#include "camera.h"
#include "debug.h"

#include <cassert>

namespace game {

World::Settings World::ms_settings;

static_assert(World::SIZE < 32);

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void World::init() {
    m_chunks.resize(DIMENSION);

    LINFO("Creating world.");

    proc::VoxMap perlin;
    proc::VoxMap voronoi;
    if (voronoi.load("assets/maps/biomes.voxmap") == false ||
        perlin.load("assets/maps/height.voxmap") == false) {
        throw std::runtime_error("Failed to load biome map.");
    }

    perlin.setScale(15.0f);

    for (u32 z = 0; z < SIDE; ++z)
        for (u32 x = 0; x < SIDE; ++x)
            m_chunks[(z * SIDE) + x].generateTerrain(perlin, voronoi, x, z);

    // TODO change
    m_origin = { SIDE * Chunk::SIZE * 0.5f, 0.0f, SIDE * Chunk::SIZE * 0.5f };
    m_origin.y = perlin.getValue(m_origin.x, m_origin.z);

    // m_worldGfx.m_portionOffset = math::ivec2(SIZE);
    // m_worldGfx.m_renderOffset = math::ivec2(CENTER_CHUNK);

    m_worldGfx.generateInstances(m_chunks);
    m_worldGfx.computeRenderedAreaChunks(m_chunks);

    LINFO("World initialized.");
}

void World::ignoreData() {
    for (auto& chunk: m_chunks) {
        chunk.clearBlocks();
    }
}

void World::setRenderDistance(u32 newDistance) noexcept {
    assert(newDistance <= SIZE && newDistance >= 0);
    ms_settings.rendering.renderDistance = newDistance;
    ms_settings.rendering.renderAreaSide = 1 + newDistance * 2;
    ms_settings.rendering.renderArea = ms_settings.rendering.renderAreaSide * ms_settings.rendering.renderAreaSide;
}
/* ========================================================================== */

const std::vector<Chunk>& World::getChunks() const noexcept {
    return m_chunks;
}

std::vector<Chunk>& World::getChunks() noexcept {
    return m_chunks;
}

Chunk& World::getChunk(const u32 x, const u32 z) noexcept {
    return m_chunks[(z * SIDE) + x];
}

const Chunk& World::getChunk(const u32 x, const u32 z) const noexcept {
    return m_chunks[(z * SIDE) + x];
}

const math::vec3& World::getOrigin() const noexcept {
    return m_origin;
}

/* GFX ====================================================================== */

void World::update(const game::Camera& camera) {
    const math::ivec2& chunkPos = camera.getChunkPosition();
    return;

    m_worldGfx.updateRenderData(chunkPos);
}

/**
 * @brief Raw block data from the world portion generated.
 * Should be used for saving/loading.
 */
std::vector<u8> World::retrieveBlocksRaw() const {
    std::vector<u8> raw;
    raw.reserve(DIMENSION * Chunk::VOLUME);

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

} // namespace game