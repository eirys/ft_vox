/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   voronoi_diagram.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 18:06:14 by etran             #+#    #+#             */
/*   Updated: 2024/06/11 23:47:42 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "other/FastNoiseLite.h"

using u32 = unsigned int;
using f32 = float;

#include <fstream>
#include <iostream>

namespace proc {

// https://auburn.github.io/FastNoiseLite/
// https://github.com/Auburn/FastNoiseLite/wiki/Documentation
void generateVoronoi() {
    FastNoiseLite noise;

    noise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Cellular);
    noise.SetSeed(VOX_SEED);
    noise.SetFrequency(0.01f);

    noise.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction::CellularDistanceFunction_Euclidean);
    noise.SetCellularReturnType(FastNoiseLite::CellularReturnType::CellularReturnType_CellValue);
    noise.SetCellularJitter(1.0f);

    const u32 size = 256;

    std::ofstream file("assets/maps/voronoi.voxmap");

    file << "VXM " << "VOR " << size << ' ';

    for (u32 y = 0; y < size; ++y) {
        for (u32 x = 0; x < size; ++x) {
            const f32 val = noise.GetNoise((f32)x, (f32)y) * 0.5f + 0.5f;
            const u32 valToU32 = *(u32*)(&val);
            file.write((char*)&valToU32, sizeof(u32));
        }
    }
}

} // namespace proc

int main() {
    proc::generateVoronoi();
}