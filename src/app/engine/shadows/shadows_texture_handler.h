/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadows_texture_handler.h                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 16:49:51 by etran             #+#    #+#             */
/*   Updated: 2023/08/23 16:49:51 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# ifndef GLFW_INCLUDE_VULKAN
#  define GLFW_INCLUDE_VULKAN
# endif

# include "texture_handler.h"

# define SHADOWMAP_SIZE 2048
# define DEPTH_FORMAT VK_FORMAT_D16_UNORM

namespace scop::graphics {

class CommandBuffer;

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

	void			init(Device& device) override;

	/* ========================================================================= */

	using super::getTextureSampler;
	using super::getTextureBuffer;

private:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void			_createTextureImages(Device& device) override;
	void			_createTextureImageView(Device& device) override;
	void			_createTextureSampler(Device& device) override;

}; // class ShadowsTextureHandler

} // namespace scop::graphics