/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_texture_handler.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 22:17:58 by etran             #+#    #+#             */
/*   Updated: 2023/08/11 23:58:05 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "texture_handler.h"

namespace scop::graphics {

class CommandBuffer;

class SceneTextureHandler final: public TextureHandler {
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

	SceneTextureHandler() = default;
	~SceneTextureHandler() = default;

	SceneTextureHandler(SceneTextureHandler&& other) = delete;
	SceneTextureHandler(const SceneTextureHandler& other) = delete;
	SceneTextureHandler& operator=(SceneTextureHandler&& other) = delete;
	SceneTextureHandler& operator=(const SceneTextureHandler& other) = delete;

	/* ========================================================================= */

	using super::destroy;

	void					init(Device& device) override;

	/* ========================================================================= */

	using super::getTextureSampler;
	using super::getTextureBuffer;

private:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void					_createTextureImages(Device& device) override;
	void					_createTextureImageView(Device& device) override;
	void					_createTextureSampler(Device& device);
	std::vector<Texture>	_loadTextures() const;

}; // class SceneTextureHandler

} // namespace scop::graphics