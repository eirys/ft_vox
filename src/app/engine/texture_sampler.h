/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_sampler.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:14:56 by etran             #+#    #+#             */
/*   Updated: 2023/06/19 17:40:39 by etran            ###   ########.fr       */
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

namespace scop {
class Image;

namespace graphics {
class Device;
class DescriptorSet;

class TextureSampler {
public:

	friend DescriptorSet;

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
		const std::vector<scop::Image>& images
	);
	void							destroy(Device& device);
	
private:
	/* ========================================================================= */
	/*                                CLASS MEMBER                               */
	/* ========================================================================= */

	uint32_t						texture_count;
	uint32_t						mip_levels;
	VkImage							vk_texture_image;
	VkDeviceMemory					vk_texture_image_memory;
	VkImageView						vk_texture_image_view;

	VkSampler						vk_texture_sampler;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void							createTextureImages(
		Device& device,
		VkCommandPool command_pool,
		const std::vector<scop::Image>& images
	);
	void							createTextureImageView(
		Device& device
	);
	void							createTextureSampler(
		Device& device
	);
	void							generateMipmaps(
		VkCommandBuffer buffer,
		Device& device,
		VkImage image,
		VkFormat image_format,
		// int32_t tex_side,
		int32_t src_size,
		uint32_t mip_level_count
	) const;

}; // class TextureSampler

/* ========================================================================== */
/*                                    OTHER                                   */
/* ========================================================================== */

void	transitionImageLayout(
	VkCommandBuffer buffer,
	VkImage image,
	VkFormat format,
	VkImageLayout old_layout,
	VkImageLayout new_layout,
	uint32_t mip_level
);

} // namespace graphics
}  // namespace scop

