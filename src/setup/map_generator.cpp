/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_generator.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 18:06:14 by etran             #+#    #+#             */
/*   Updated: 2024/08/20 14:09:42 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libs/procedural/other/FastNoiseLite.h"
#include "../engine/game/game_decl.h"

using u8 = unsigned char;
using u32 = unsigned int;
using f32 = float;

#include <fstream>
#include <iostream>

static constexpr bool DEBUG = true;

namespace proc {

static constexpr u32 size = CHUNK_SIZE * RENDER_DISTANCE;

template <typename F, typename G>
static
void fillFile(
    const FastNoiseLite& noise,
    std::ofstream& file,
    F writeToFile,
    G extra
) {
    for (u32 y = 0; y < size; ++y) {
        for (u32 x = 0; x < size; ++x) {
            f32 x2 = (f32)x, y2 = (f32)y;
            extra(noise, x2, y2);
            const f32 val = noise.GetNoise(x2, y2) * 0.5f + 0.5f;
            writeToFile(file, val);
        }
    }
};

template <typename F, typename G>
static
void createMap(
    const FastNoiseLite& noise,
    const std::string& name,
    const std::string& type,
    F extra,
    G writeFile
) {
    std::ofstream file("assets/maps/" + name + ".voxmap");
    file << "VXM " << type << ' ' << size << ' ';

    fillFile(
        noise,
        file,
        writeFile,
        extra
    );
}

template <typename F>
static
void debug(
    const FastNoiseLite& noise,
    const std::string& name,
    F extra
) {
    std::ofstream file("assets/maps/" + name + "_debug.ppm");

    file << "P6\n" << size << ' ' << size << "\n255\n";

    fillFile(
        noise,
        file,
        [](std::ofstream& file, f32 val) -> void {
            const u8 tmp = (u8)(val * 255.0f);
            file << tmp << tmp << tmp;
        },
        extra
    );
}

// https://auburn.github.io/FastNoiseLite/
// https://github.com/Auburn/FastNoiseLite/wiki/Documentation
static
void generateBiomeMap() {
    FastNoiseLite voroNoise;

    voroNoise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Cellular);
    voroNoise.SetSeed(VOX_SEED);
    voroNoise.SetFrequency(0.01f);

    voroNoise.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction::CellularDistanceFunction_Euclidean);
    voroNoise.SetCellularReturnType(FastNoiseLite::CellularReturnType::CellularReturnType_CellValue);
    voroNoise.SetCellularJitter(1.0f);

    voroNoise.SetDomainWarpType(FastNoiseLite::DomainWarpType::DomainWarpType_BasicGrid);
    voroNoise.SetDomainWarpAmp(150.0f);

    if (DEBUG) {
        debug(voroNoise, "biomes",
        [](const FastNoiseLite& noise, f32& x, f32& y) {
            noise.DomainWarp(x, y);
        });
    } else {
        createMap(voroNoise, "biomes", "VOR",
        [](const FastNoiseLite& noise, f32& x, f32& y) {
            noise.DomainWarp(x, y);
        },
        [](std::ofstream& file, f32 val) -> void {
            u32 tmp = *(u32*)(&val);
            file.write((char*)&tmp, 4u);
        });
    }
}

static
void generateHeightMap() {
    FastNoiseLite perlinNoise;

    perlinNoise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Perlin);
    perlinNoise.SetSeed(VOX_SEED);
    perlinNoise.SetFrequency(0.05f);

    perlinNoise.SetFractalType(FastNoiseLite::FractalType::FractalType_FBm);
    perlinNoise.SetFractalOctaves(4);
    perlinNoise.SetFractalLacunarity(2.0f);
    perlinNoise.SetFractalGain(0.5f);
    perlinNoise.SetFractalWeightedStrength(0.5f);

    if (DEBUG) {
        debug(perlinNoise, "height",
        [](const FastNoiseLite& noise, f32& x, f32& y) {});
    } else {
        createMap(perlinNoise, "height", "PER",
        [](const FastNoiseLite& noise, f32& x, f32& y) {},
        [](std::ofstream& file, f32 val) -> void {
            u32 tmp = *(u32*)(&val);
            file.write((char*)&tmp, 4u);
        });
    }
}

} // namespace proc

int main() {
    proc::generateBiomeMap();
    proc::generateHeightMap();
}