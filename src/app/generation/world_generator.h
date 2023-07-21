/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world_generator.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 22:54:18 by etran             #+#    #+#             */
/*   Updated: 2023/07/16 15:26:12 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "perlin_noise.h"

namespace vox {

/**
 * @brief Generates a world using perlin noise.
 * Handles the generation of the world's chunks.
*/
class WorldGenerator {
public:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	WorldGenerator();
	~WorldGenerator();

	WorldGenerator(WorldGenerator&& src) = delete;
	WorldGenerator(const WorldGenerator& src) = delete;
	WorldGenerator &operator=(WorldGenerator&& rhs) = delete;
	WorldGenerator &operator=(const WorldGenerator& rhs) = delete;

	/* ========================================================================= */

private:
	/* ========================================================================= */
	/*                               CONST MEMBERS                               */
	/* ========================================================================= */

	static constexpr const std::size_t	seed = 42;
	static constexpr const std::size_t	chunk_size = 16;
	static constexpr const std::size_t	render_distance = 16;

	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	PerlinNoise		_noise;
	std::size_t		_offset = 0; // Updated as the player moves.

}; // class WorldGenerator

} // namespace vox