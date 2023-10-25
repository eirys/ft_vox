/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world_generator.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 12:06:07 by etran             #+#    #+#             */
/*   Updated: 2023/09/22 10:40:45 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "world_generator.h"
#include "world.h"

namespace vox {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

WorldGenerator::WorldGenerator(std::size_t seed):
	_noise(_getDefaultMapInfo(seed)) {}

/* ========================================================================== */

World		WorldGenerator::generate() const noexcept {
	return World(_noise, _width, _height);
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

PerlinNoise::NoiseMapInfo	WorldGenerator::_getDefaultMapInfo(
	std::size_t seed
) const noexcept {
	PerlinNoise::NoiseMapInfo	noise_info{};
	noise_info.type = vox::PerlinNoiseType::PERLIN_NOISE_2D;
	noise_info.seed.emplace(static_cast<uint32_t>(seed));

	// World sizes
	noise_info.width = _width;
	noise_info.height = _height;
	noise_info.depth = _depth;

	// Noise generation info
	noise_info.layers = 3;
	noise_info.frequency_0 = .03f;
	noise_info.frequency_mult = 1.8f;
	noise_info.amplitude_mult = 0.5f;
	return noise_info;
}

} // namespace vox