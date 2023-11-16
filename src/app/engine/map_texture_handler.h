/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_texture_handler.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 17:10:13 by etran             #+#    #+#             */
/*   Updated: 2023/11/02 17:10:13 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "texture_handler.h"
# include "chunk_macros.h"

namespace scop::gfx {

/**
 * @brief Contains data for chunk presence.
*/
class MapTextureHandler final: public TextureHandler {
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

	MapTextureHandler() = default;
	~MapTextureHandler() = default;

	MapTextureHandler(MapTextureHandler&& other) = delete;
	MapTextureHandler(const MapTextureHandler& other) = delete;
	MapTextureHandler& operator=(MapTextureHandler&& other) = delete;
	MapTextureHandler& operator=(const MapTextureHandler& other) = delete;

	/* ========================================================================= */

	void					init(Device& device) override;
	void					destroy(Device& device) override;
	void					copyData(
		Device& device,
		const std::array<uint16_t, RENDER_DISTANCE2>& chunk_map);


	/* ========================================================================= */

	using super::getTextureSampler;
	using super::getTextureBuffer;

	/* ========================================================================= */

	using super::setTextureSampler;

private:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void					_createTextureImages(Device& device) override;
	void					_createTextureImageView(Device& device) override;

}; // class MapTextureHandler

} // namespace scop::gfx