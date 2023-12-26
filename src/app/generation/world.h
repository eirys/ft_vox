/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 22:53:11 by etran             #+#    #+#             */
/*   Updated: 2023/12/24 15:29:11 by etran            ###   ########.fr       */
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

	World(const PerlinNoise& noise, std::size_t width, std::size_t height);
	World(World &&src);
	World &operator=(World &&rhs);

	World() = default;
	~World() = default;

	World(const World &src) = delete;
	World &operator=(const World &rhs) = delete;

	/* ========================================================================= */

	// DEPRECATED
	std::vector<uint8_t>			generateHeightBuffer() const noexcept;

	void							updateTerrainData(std::vector<uint16_t>& data) const;

	/* ========================================================================== */

	void							setOrigin(const scop::Vect3& origin) noexcept;
	const scop::Vect3&				getOrigin() const noexcept;

	const std::vector<Chunk>&		getChunks() const noexcept;
	const Chunk&					getChunk(uint8_t x, uint8_t y, uint8_t z) const;
	Chunk&							getChunk(uint8_t x, uint8_t y, uint8_t z);

	const Block&					getBlock(float x, float y, float z) const;
	Block&							getBlock(float x, float y, float z);
	const Block&					getBlock(const scop::Vect3& pos) const;
	Block&							getBlock(const scop::Vect3& pos);

	float							getWidth() const noexcept;
	float							getDepth() const noexcept;
	uint8_t							getRenderDistance() const noexcept;

	const std::vector<uint16_t>&	getWorldData() const noexcept;
	std::vector<uint16_t>&			getWorldData() noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	scop::Vect3						_origin = {0.0f, 0.0f, 0.0f};
	std::vector<Chunk>				_chunks;
	std::vector<uint16_t>			_world_data;
	// Light todo put here

	// For now, only render distance is rendered
	static constexpr const uint8_t	_render_distance = RENDER_DISTANCE;
	static constexpr const float	_world_width = _render_distance * CHUNK_SIZE;
	static constexpr const float	_world_depth = _render_distance * CHUNK_SIZE;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void							_generateChunks(const PerlinNoise& noise);
	void							_generateTerrainData();

}; // class World

} // namespace vox