/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:51:38 by etran             #+#    #+#             */
/*   Updated: 2024/03/17 22:13:59 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "world.h"
#include "perlin_noise.h"

namespace game {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void World::init(const u32 seed) {
    proc::NoiseMapInfo info{};
    info.seed = seed;
    info.type = proc::PerlinNoiseType::PERLIN_NOISE_2D;
    info.width = WORLD_WIDTH * CHUNK_SIZE;
    info.height = WORLD_DEPTH * CHUNK_SIZE;
    info.layers = 3;
    info.frequency_0 = 0.03f;
    info.frequency_mult = 2.0f;
    info.amplitude_mult = 0.5f;

    proc::PerlinNoise perlin(info);

    for (u32 z = 0; z < WORLD_DEPTH; ++z) {
        for (u32 x = 0; x < WORLD_WIDTH; ++x) {
            Chunk& chunk = m_chunks[z * WORLD_WIDTH + x];

            chunk.generate(perlin, x * CHUNK_SIZE, z * CHUNK_SIZE);
        }
    }
}

const std::array<Chunk, WORLD_SIZE>& World::getChunks() const noexcept {
    return m_chunks;
}

std::array<Chunk, WORLD_SIZE>& World::getChunks() noexcept {
    return m_chunks;
}

} // namespace game