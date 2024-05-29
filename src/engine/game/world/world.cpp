/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:51:38 by etran             #+#    #+#             */
/*   Updated: 2024/05/29 11:12:25 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "world.h"
#include "perlin_noise.h"
#include "debug.h"

namespace game {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void World::init(const u32 seed) {
    proc::NoiseMapInfo info{};
    info.seed = seed;
    info.type = proc::PerlinNoiseType::PERLIN_NOISE_2D;
    info.width = RENDER_DISTANCE * CHUNK_SIZE;
    info.height = RENDER_DISTANCE * CHUNK_SIZE;
    info.layers = 3;
    info.frequency_0 = 0.03f;
    info.frequency_mult = 2.0f;
    info.amplitude_mult = 0.5f;

    proc::PerlinNoise perlin(info);

    for (u32 z = 0; z < RENDER_DISTANCE; ++z) {
        for (u32 x = 0; x < RENDER_DISTANCE; ++x) {
            for (u32 y = 0; y < RENDER_HEIGHT; ++y) {
                m_chunks[(y * RENDER_AREA) + (z * RENDER_DISTANCE) + x].generate(perlin, x, y, z);
            }
        }
    }

    LINFO("World initialized");
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

} // namespace game