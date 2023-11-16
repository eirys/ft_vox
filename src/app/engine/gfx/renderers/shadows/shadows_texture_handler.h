/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadows_texture_handler.h                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 16:49:51 by etran             #+#    #+#             */
/*   Updated: 2023/11/16 23:32:55 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# include <vulkan/vulkan.h>

# include "texture_handler.h"

namespace scop::gfx {

class ShadowsTextureHandler final: public TextureHandler {
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

	ShadowsTextureHandler() = default;
	~ShadowsTextureHandler() = default;

	ShadowsTextureHandler(ShadowsTextureHandler&& other) = delete;
	ShadowsTextureHandler(const ShadowsTextureHandler& other) = delete;
	ShadowsTextureHandler& operator=(ShadowsTextureHandler&& other) = delete;
	ShadowsTextureHandler& operator=(const ShadowsTextureHandler& other) = delete;

	/* ========================================================================= */

	using super::destroy;

	void			init(scop::core::Device& device) override;

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

}; // class ShadowsTextureHandler

} // namespace scop::gfx