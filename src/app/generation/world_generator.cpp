/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world_generator.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 12:06:07 by etran             #+#    #+#             */
/*   Updated: 2023/09/18 17:34:06 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "world_generator.h"
#include "world.h"

namespace vox {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

World		WorldGenerator::generate(std::size_t seed) const noexcept {
	PerlinNoise	noise({
		.type = vox::PerlinNoiseType::PERLIN_NOISE_2D,
		.seed = seed,
		.width = render_distance * chunk_size,
		.height = render_distance * chunk_size,
		.depth = 2 * chunk_size,
		.layers = 3,
		.frequency_0 = .03f,
		.frequency_mult = 1.8f,
		.amplitude_mult = 0.5f });

	return World(noise);
}

} // namespace vox