/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 22:53:11 by etran             #+#    #+#             */
/*   Updated: 2023/09/25 14:42:37 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Std
# include <vector> // std::vector

# include "chunk.h"

# define WORLD_WIDTH	1024 * CHUNK_SIZE				// 16384
# define WORLD_DEPTH	WORLD_WIDTH						// 16384
# define WORLD_HEIGHT	256

# define WORLD_SIZE		WORLD_WIDTH * WORLD_DEPTH		// 268435456
# define WORLD_VOLUME	WORLD_SIZE * WORLD_HEIGHT		// 68719476736

namespace vox {

class PerlinNoise;

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

	World() = default;
	World(World &&src) = default;
	World &operator=(World &&rhs) = default;
	~World() = default;

	World(const World &src) = delete;
	World &operator=(const World &rhs) = delete;

	/* ========================================================================= */

	std::vector<uint8_t>		generateHeightBuffer() const noexcept;

	/* ========================================================================== */

	void						setOrigin(const scop::Vect3& origin) noexcept;
	const scop::Vect3&			getOrigin() const noexcept;

	const std::vector<Chunk>&	getChunks() const noexcept;
	const Chunk&				getChunk(uint8_t x, uint8_t y, uint8_t z) const noexcept;
	Chunk&						getChunk(uint8_t x, uint8_t y, uint8_t z) noexcept;

	uint32_t					getWidth() const noexcept;
	uint32_t					getDepth() const noexcept;


private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	scop::Vect3						_origin = {0.0f, 0.0f, 0.0f};
	std::vector<Chunk>				_chunks;

	// For now, only render distance is rendered
	static constexpr const uint32_t	_world_width = RENDER_DISTANCE;
	static constexpr const uint32_t	_world_depth = RENDER_DISTANCE;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void					_generateChunks(const PerlinNoise& noise);

}; // class World

} // namespace vox