/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_texture_handler.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 14:32:13 by etran             #+#    #+#             */
/*   Updated: 2023/11/16 22:27:13 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "texture_handler.h"

namespace scop::gfx {

/**
 * @brief Contains data for each block for each chunk.
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
		const std::vector<uint8_t>& height_map);

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