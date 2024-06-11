/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:51:38 by etran             #+#    #+#             */
/*   Updated: 2024/06/11 13:24:04 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "world.h"
#include "perlin_noise.h"
#include "voxmap.h"

#include "debug.h"

namespace game {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void World::init(const u32 seed) {
    proc::NoiseMapInfo noiseInfo{};
    noiseInfo.seed = seed;
    noiseInfo.type = proc::PerlinNoiseType::PERLIN_NOISE_2D;
    noiseInfo.width = RENDER_DISTANCE * CHUNK_SIZE;
    noiseInfo.height = RENDER_DISTANCE * CHUNK_SIZE;
    noiseInfo.layers = 3;
    noiseInfo.frequency_0 = 0.05f;
    noiseInfo.frequency_mult = 2.0f;
    noiseInfo.amplitude_mult = 0.5f;
    noiseInfo.scale = CHUNK_SIZE - 1.0f;
    proc::PerlinNoise perlin(noiseInfo);

    proc::VoronoiDiagram voronoi;
    if (voronoi.load("assets/maps/voronoi.voxmap") == false) {
        throw std::runtime_error("Failed to load voronoi map.");
    }

    // voronoi.SetNoiseType(proc::VoronoiDiagram::NoiseType::NoiseType_Cellular);
    // voronoi.SetSeed(seed);
    // voronoi.SetFrequency(0.01f);
    // voronoi.SetCellularDistanceFunction(proc::VoronoiDiagram::CellularDistanceFunction::CellularDistanceFunction_Euclidean);
    // voronoi.SetCellularReturnType(proc::VoronoiDiagram::CellularReturnType::CellularReturnType_CellValue);
    // voronoi.SetCellularJitter(1.0f);

    for (u32 z = 0; z < RENDER_DISTANCE; ++z) {
        for (u32 x = 0; x < RENDER_DISTANCE; ++x) {
            for (u32 y = 0; y < RENDER_HEIGHT; ++y) {
                m_chunks[(y * RENDER_AREA) + (z * RENDER_DISTANCE) + x].generate(perlin, voronoi, x, y, z);
            }
        }
    }

    m_origin = WORLD_ORIGIN;
    m_origin.y = perlin.noiseAt(m_origin.x, m_origin.z);

    LINFO("World initialized.");
}

/* ========================================================================== */

const World::ChunkArray& World::getChunks() const noexcept {
    return m_chunks;
}

World::ChunkArray& World::getChunks() noexcept {
    return m_chunks;
}

Chunk& World::getChunk(const u32 x, const u32 y, const u32 z) noexcept {
    return m_chunks[(y * RENDER_AREA) + (z * RENDER_DISTANCE) + x];
}

const Chunk& World::getChunk(const u32 x, const u32 y, const u32 z) const noexcept {
    return m_chunks[(y * RENDER_AREA) + (z * RENDER_DISTANCE) + x];
}

const math::Vect3& World::getOrigin() const noexcept {
    return m_origin;
}

} // namespace game