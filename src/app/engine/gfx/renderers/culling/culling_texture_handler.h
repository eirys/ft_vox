/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   culling_texture_handler.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 17:10:13 by etran             #+#    #+#             */
/*   Updated: 2023/12/05 17:10:11 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "texture_handler.h"
# include "chunk_macros.h"

namespace scop::gfx {

/**
 * @brief Culling data texture handler.
*/
class CullingTextureHandler final: public TextureHandler {
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

	CullingTextureHandler() = default;
	~CullingTextureHandler() = default;

	CullingTextureHandler(CullingTextureHandler&& other) = delete;
	CullingTextureHandler(const CullingTextureHandler& other) = delete;
	CullingTextureHandler& operator=(CullingTextureHandler&& other) = delete;
	CullingTextureHandler& operator=(const CullingTextureHandler& other) = delete;

	/* ========================================================================= */

	void					init(scop::core::Device& device) override;
	void					destroy(scop::core::Device& device) override;
	void					copyData(
		scop::core::Device& device,
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

	void					_createTextureImages(scop::core::Device& device) override;
	void					_createTextureImageView(scop::core::Device& device) override;

}; // class CullingTextureHandler

} // namespace scop::gfx