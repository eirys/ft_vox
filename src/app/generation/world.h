/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 22:53:11 by etran             #+#    #+#             */
/*   Updated: 2023/09/18 17:51:08 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// #include <array> // std::array
# include <vector> // std::vector

# include "chunk.h"
# include "perlin_noise.h"

# define WORLD_WIDTH	1024 * CHUNK_SIZE				// 16384
# define WORLD_DEPTH	WORLD_WIDTH						// 16384
# define WORLD_HEIGHT	256

# define WORLD_SIZE		WORLD_WIDTH * WORLD_DEPTH		// 268435456
# define WORLD_VOLUME	WORLD_SIZE * WORLD_HEIGHT		// 68719476736

namespace vox {

/**
 * @brief Contains data on the world (block position, state, etc).
*/
class World final {
public:
	/* ========================================================================= */
	/*                                   PUBLIC                                  */
	/* ========================================================================= */

	World(
		const PerlinNoise& noise,
		std::size_t width,
		std::size_t height);

	World(World &&src) = default;
	World &operator=(World &&rhs) = default;
	~World() = default;

	World() = delete;
	World(const World &src) = delete;
	World &operator=(const World &rhs) = delete;

	/* ========================================================================= */

	void				setOrigin(const scop::Vect3& origin) noexcept;
	const scop::Vect3&	getOrigin() const noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	// TODO: Need to store it differently...
	// std::array<Chunk, WORLD_VOLUME>	_chunks;

	scop::Vect3			_origin = {0.0f, 0.0f, 0.0f};

	// Temporary
	std::vector<Chunk>	_chunks;

}; // class World

} // namespace vox