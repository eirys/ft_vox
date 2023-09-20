/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world_generator.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 22:54:18 by etran             #+#    #+#             */
/*   Updated: 2023/09/18 17:31:06 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "perlin_noise.h"

namespace vox {

class World;

/**
 * @brief Generates a world using perlin noise.
 *
 * Responsible for world update as the player advances in the world.
*/
class WorldGenerator {
public:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	WorldGenerator(std::size_t seed);

	WorldGenerator(WorldGenerator &&src) = default;
	~WorldGenerator() = default;

	WorldGenerator &operator=(WorldGenerator &&rhs) = delete;
	WorldGenerator(const WorldGenerator &src) = delete;
	WorldGenerator &operator=(const WorldGenerator &rhs) = delete;

	/* ========================================================================= */

	World		generate() const noexcept;

	/* ========================================================================= */

	// void		updateWorld(World& world /* , Data */) const noexcept;
	std::array<float, CHUNK_AREA>	generateHeightBuffer() const noexcept;
	PerlinNoise::PerlinMesh			toPerlinMesh() const noexcept; // Temporary

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	std::size_t		_offset = 0; // Updated as the player moves.
	PerlinNoise		_noise;

	uint32_t		_width = RENDER_DISTANCE * CHUNK_SIZE;
	uint32_t		_height = RENDER_DISTANCE * CHUNK_SIZE;
	uint32_t		_depth = 2 * CHUNK_SIZE;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	PerlinNoise::NoiseMapInfo	_getDefaultMapInfo(std::size_t seed) const noexcept;

}; // class WorldGenerator

} // namespace vox