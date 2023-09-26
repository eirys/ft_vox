/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_handler.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:14:56 by etran             #+#    #+#             */
/*   Updated: 2023/08/12 23:38:18 by etran            ###   ########.fr       */
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

# define TEXTURE_SAMPLER_COUNT 16

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

	typedef	::scop::Image			Texture;
	typedef	std::vector<Texture>	TextureArray;
	typedef	std::array<Texture, 6>	CubeMap;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	virtual ~TextureHandler() = default;

	/* ========================================================================= */

	virtual void			init(Device& device) = 0;
	void					destroy(Device& device);

	/* ========================================================================= */

	VkSampler				getTextureSampler() const noexcept;
	const ImageBuffer&		getTextureBuffer() const noexcept;

protected:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	uint32_t				_texture_count;
	uint32_t				_layer_count;
	uint32_t				_mip_levels = 1;

	ImageBuffer				_texture_buffer;
	VkSampler				_texture_sampler;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	TextureHandler() = default;

	TextureHandler(TextureHandler&& other) = delete;
	TextureHandler(const TextureHandler& other) = delete;
	TextureHandler& operator=(TextureHandler&& other) = delete;
	TextureHandler& operator=(const TextureHandler& other) = delete;

	/* ========================================================================= */

	virtual void			_createTextureImages(Device& device) = 0;
	virtual void			_createTextureImageView(Device& device) = 0;
	virtual void			_createTextureSampler(Device& device) = 0;

	uint32_t				_getMipLevelCount(uint32_t image_width) const;

}; // class TextureHandler

} // namespace scop::graphics