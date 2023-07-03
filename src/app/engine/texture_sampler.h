/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_sampler.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:14:56 by etran             #+#    #+#             */
/*   Updated: 2023/07/03 09:40:21 by etran            ###   ########.fr       */
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
class DescriptorSet;

class TextureSampler {
public:

	friend DescriptorSet;

	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	typedef	std::array<scop::Image, 6>		CubeMap;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	TextureSampler() = default;
	TextureSampler(TextureSampler&& other) = default;
	~TextureSampler() = default;

	TextureSampler(const TextureSampler& other) = delete;
	TextureSampler& operator=(const TextureSampler& other) = delete;

	/* ========================================================================= */

	void							init(
		Device& device,
		VkCommandPool command_pool,
		const std::vector<CubeMap>& images
	);
	void							destroy(Device& device);
	
private:
	/* ========================================================================= */
	/*                                CLASS MEMBER                               */
	/* ========================================================================= */

	uint32_t						texture_count;
	uint32_t						mip_levels;

	ImageBuffer						texture_buffer;
	VkSampler						vk_texture_sampler;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void							createTextureImages(
		Device& device,
		VkCommandPool command_pool,
		const std::vector<CubeMap>& images
	);
	void							createTextureImageView(
		Device& device
	);
	void							createTextureSampler(
		Device& device
	);

}; // class TextureSampler

} // namespace graphics
} // namespace scop

