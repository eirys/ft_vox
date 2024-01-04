/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_texture_handler.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 14:32:13 by etran             #+#    #+#             */
/*   Updated: 2023/12/28 19:30:41 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "texture_handler.h"

namespace scop::gfx {

/* ========================================================================== */
/*                                HELPER CLASS                                */
/* ========================================================================== */

/**
 * @brief Used to offset the texture buffer top left corner.
*/
class Travelator final {
public:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	Travelator() = default;
	~Travelator() = default;

	/* ========================================================================= */

	void			reset() { _x = 0; _y = 0; _z = 0; }

	void			incrementX() { ++_x; }
	void			incrementX(uint32_t x) { _x += x; }
	void			incrementY() { ++_y; }
	void			incrementY(uint32_t y) { _y += y; }
	void			incrementZ() { ++_z; }
	void			incrementZ(uint32_t z) { _z += z; }

	void			decrementX() { --_x; }
	void			decrementX(uint32_t x) { _x -= x; }
	void			decrementY() { --_y; }
	void			decrementY(uint32_t y) { _y -= y; }
	void			decrementZ() { --_z; }
	void			decrementZ(uint32_t z) { _z -= z; }

	uint32_t		getX() const { return _x; }
	uint32_t		getY() const { return _y; }
	uint32_t		getZ() const { return _z; }

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	// Offsets
	uint32_t		_x = 0;
	uint32_t		_y = 0;
	uint32_t		_z = 0;

};

/**
 * @brief Contains chunk data, aka blocks type
*/
class ChunkTextureHandler final: public TextureHandler {
public:
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	using super = TextureHandler;
	using super::Texture;
	using super::TextureArray;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	ChunkTextureHandler() = default;
	~ChunkTextureHandler() = default;

	ChunkTextureHandler(ChunkTextureHandler&& other) = delete;
	ChunkTextureHandler(const ChunkTextureHandler& other) = delete;
	ChunkTextureHandler& operator=(ChunkTextureHandler&& other) = delete;
	ChunkTextureHandler& operator=(const ChunkTextureHandler& other) = delete;

	/* ========================================================================= */

	using super::destroy;

	void			init(scop::core::Device& device) override;
	void			copyData(
		scop::core::Device& device,
		const std::vector<uint16_t>& blocks_data);

	void			updateData(
		scop::core::Device& device,
		const std::vector<uint16_t>& blocks_data,
		const Travelator& travelator);

	/* ========================================================================= */

	using super::getTextureSampler;
	using super::getTextureBuffer;

private:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void			_createTextureImages(scop::core::Device& device) override;
	void			_createTextureImageView(scop::core::Device& device) override;
	void			_createTextureSampler(scop::core::Device& device);

}; // class ChunkTextureHandler

} // namespace scop::gfx