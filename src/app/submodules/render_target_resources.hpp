/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_target_resources.hpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 20:07:43 by etran             #+#    #+#             */
/*   Updated: 2023/05/16 14:37:01 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEPTH_STENCIL_HPP
# define DEPTH_STENCIL_HPP

// Graphics
# ifndef GLFW_INCLUDE_VULKAN
#  define GLFW_INCLUDE_VULKAN
# endif

# include <GLFW/glfw3.h>

// Std
# include <vector> // std::vector

namespace scop {
namespace graphics {

class Device;
class RenderTarget;

class RenderTargetResources {
public:

	friend class RenderTarget;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	RenderTargetResources() = default;
	RenderTargetResources(RenderTargetResources&& other) = default;
	~RenderTargetResources() = default;

	RenderTargetResources(const RenderTargetResources& other) = delete;
	RenderTargetResources& operator=(const RenderTargetResources& other) = delete;

	/* ========================================================================= */

	void							init(
		Device& device,
		VkExtent2D swap_chain_extent,
		VkFormat swap_chain_image_format
	);

	void							destroy(Device& device);

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	VkImage							depth_image;
	VkDeviceMemory					depth_image_memory;
	VkImageView						depth_image_view;

	VkImage							color_image;
	VkDeviceMemory					color_image_memory;
	VkImageView						color_image_view;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void							createColorResources(
		Device& device,
		VkExtent2D extent,
		VkFormat image_format
	);
	void							createDepthResources(
		Device& device,
		VkExtent2D extent
	);

}; // class DepthStencil

} // namespace graphics
}  // namespace scop

#endif