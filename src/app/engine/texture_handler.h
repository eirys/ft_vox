/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_handler.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:14:56 by etran             #+#    #+#             */
/*   Updated: 2023/08/10 22:12:50 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# ifndef GLFW_INCLUDE_VULKAN
#  define GLFW_INCLUDE_VULKAN
# endif
# include <GLFW/glfw3.h>

// Std
# include <vector> // std::vector
# include <array> // std::array

# include "image_buffer.h"

namespace scop {
class Image;
} // namespace scop

namespace scop::graphics {

class Device;
class CommandPool;

class TextureHandler {
public:
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	typedef	scop::Image				Texture;
	typedef	std::vector<Texture>	TextureArray;
	typedef	std::array<Texture, 6>	CubeMap;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	TextureHandler() = default;
	~TextureHandler() = default;

	TextureHandler(TextureHandler&& other) = delete;
	TextureHandler(const TextureHandler& other) = delete;
	TextureHandler& operator=(TextureHandler&& other) = delete;
	TextureHandler& operator=(const TextureHandler& other) = delete;

	/* ========================================================================= */

	virtual void			init(
		Device& device,
		CommandPool& command_pool,
		const std::vector<Texture>& images) = 0;
	virtual void			destroy(Device& device) = 0;

	/* ========================================================================= */

	VkSampler				getTextureSampler() const noexcept;
	const ImageBuffer&		getTextureBuffer() const noexcept;

protected:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	uint32_t				_texture_count;
	const uint32_t			_layer_count = 1; // 6 for cube map
	uint32_t				_mip_levels;

	ImageBuffer				_texture_buffer;
	VkSampler				_vk_texture_sampler;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	virtual void			_createTextureImages(
		Device& device,
		CommandPool& command_pool,
		const std::vector<Texture>& images) = 0;
	virtual void			_createTextureImageView(Device& device) = 0;
	virtual void			_createTextureSampler(Device& device) = 0;

}; // class TextureHandler

} // namespace scop::graphics