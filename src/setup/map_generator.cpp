/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_generator.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 18:06:14 by etran             #+#    #+#             */
/*   Updated: 2024/06/13 19:07:00 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libs/procedural/other/FastNoiseLite.h"
#include "../engine/game/game_decl.h"

using u8 = unsigned char;
using u32 = unsigned int;
using f32 = float;

#include <fstream>
#include <iostream>

static constexpr bool DEBUG = false;

namespace proc {

static constexpr u32 size = CHUNK_SIZE * RENDER_DISTANCE;

static FastNoiseLite noise;

template <typename F>
static
void fillFile(std::ofstream& file, u32 eleSize, F f) {
    for (u32 y = 0; y < size; ++y) {
        for (u32 x = 0; x < size; ++x) {
            f32 x2 = (f32)x, y2 = (f32)y;
            noise.DomainWarp(x2, y2);
            const f32 val = noise.GetNoise(x2, y2) * 0.5f + 0.5f;
            auto actualValue = f(val);
            file.write((char*)&actualValue, eleSize);
        }
    }
};

static
void debug(const FastNoiseLite& noise, u32 size) {
    std::ofstream file("assets/maps/biomes_debug.ppm");

    file << "P6\n" << 256 << ' ' << 256 << "\n255\n";

    fillFile(file, 3U, [](f32 val) -> u32 {
        const u32 tmp = (u8)(val * 255.0f);
        return tmp << 16 | tmp << 8 | tmp;
    });
}

// https://auburn.github.io/FastNoiseLite/
// https://github.com/Auburn/FastNoiseLite/wiki/Documentation
static
void setVoronoi() {
    noise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Cellular);
    noise.SetSeed(VOX_SEED);
    noise.SetFrequency(0.01f);

    noise.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction::CellularDistanceFunction_Euclidean);
    noise.SetCellularReturnType(FastNoiseLite::CellularReturnType::CellularReturnType_CellValue);
    noise.SetCellularJitter(1.0f);

    noise.SetDomainWarpType(FastNoiseLite::DomainWarpType::DomainWarpType_BasicGrid);
    noise.SetDomainWarpAmp(150.0f);
}

static
void generateBiomeMap() {
    if (DEBUG) {
        debug(noise, size);
    } else {
        std::ofstream file("assets/maps/biomes.voxmap");

        file << "VXM " << "VOR " << size << ' ';

        fillFile(file, 4U, [](f32 val) -> u32 {
            return *(u32*)(&val);
        });
    }
}

static
void generateHeightMap() {
    if (DEBUG) {
        debug(noise, size);
    } else {
        std::ofstream file("assets/maps/heigtmap.voxmap");

        file << "VXM " << "PER " << size << ' ';

        fillFile(file, 4U, [](f32 val) -> u32 {
            return *(u32*)(&val);
        });
    }
}

} // namespace proc

int main() {
    proc::setVoronoi();
    proc::generateBiomeMap();
}