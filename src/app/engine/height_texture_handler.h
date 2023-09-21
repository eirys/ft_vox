/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   height_texture_handler.h                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 11:11:02 by etran             #+#    #+#             */
/*   Updated: 2023/09/21 11:11:02 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "texture_handler.h"

namespace scop::graphics {

class CommandBuffer;

/**
 * @brief Contains data for each block for each chunk.
*/
class HeightTextureHandler final: public TextureHandler {
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

	HeightTextureHandler() = default;
	~HeightTextureHandler() = default;

	HeightTextureHandler(HeightTextureHandler&& other) = delete;
	HeightTextureHandler(const HeightTextureHandler& other) = delete;
	HeightTextureHandler& operator=(HeightTextureHandler&& other) = delete;
	HeightTextureHandler& operator=(const HeightTextureHandler& other) = delete;

	/* ========================================================================= */

	using super::destroy;

	void					init(Device& device) override;
	void					copyData(
		Device& device,
		const std::vector<uint8_t>& height_map);

	/* ========================================================================= */

	using super::getTextureSampler;
	using super::getTextureBuffer;

private:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void					_createTextureImages(Device& device) override;
	void					_createTextureImageView(Device& device) override;
	void					_createTextureSampler(Device& device) override;

}; // class HeightTextureHandler

} // namespace scop::graphics