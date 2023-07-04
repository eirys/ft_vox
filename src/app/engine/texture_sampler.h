/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_sampler.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:14:56 by etran             #+#    #+#             */
/*   Updated: 2023/07/04 09:28:02 by etran            ###   ########.fr       */
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

namespace graphics {
class Device;
class CommandPool;

class TextureSampler {
public:
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	typedef	std::array<scop::Image, 6>	CubeMap;
	typedef	std::vector<scop::Image>	TextureArray;

	typedef	scop::Image					Texture;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	TextureSampler() = default;
	~TextureSampler() = default;

	TextureSampler(TextureSampler&& other) = delete;
	TextureSampler(const TextureSampler& other) = delete;
	TextureSampler& operator=(TextureSampler&& other) = delete;
	TextureSampler& operator=(const TextureSampler& other) = delete;

	/* ========================================================================= */

	void							init(
		Device& device,
		CommandPool& command_pool,
		const std::vector<Texture>& images
	);
	void							destroy(Device& device);

	/* ========================================================================= */

	VkSampler						getTextureSampler() const noexcept;
	const ImageBuffer&				getTextureBuffer() const noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	uint32_t						_texture_count;
	const uint32_t					_layer_count = 1; // 6 for cube map
	uint32_t						_mip_levels;

	ImageBuffer						_texture_buffer;
	VkSampler						_vk_texture_sampler;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void							_createTextureImages(
		Device& device,
		CommandPool& command_pool,
		const std::vector<Texture>& images
	);
	void							_createTextureImageView(
		Device& device
	);
	void							_createTextureSampler(
		Device& device
	);

}; // class TextureSampler

} // namespace graphics
} // namespace scop

