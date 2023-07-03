/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap_chain.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/02 16:27:11 by etran             #+#    #+#             */
/*   Updated: 2023/07/03 10:10:19 by etran            ###   ########.fr       */
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

# include "image_buffer.h"

namespace scop {
class Window;

namespace graphics {
class Device;
class RenderPass;

/**
 * @brief Wrapper class for swap chain images
*/
class SwapChain {
public:

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	SwapChain() = default;
	~SwapChain() = default;

	SwapChain(SwapChain&& other) = delete;
	SwapChain(const SwapChain& other) = delete;
	SwapChain& operator=(SwapChain&& other) = default;
	SwapChain& operator=(const SwapChain& other) = delete;

	/* ========================================================================= */

	void								init(
		Device& device,
		scop::Window& window
	);

	void								initFrameBuffers(
		Device& device,
		RenderPass& render_pass
	);

	void								destroy(Device& device);

	void								update(
		Device& device,
		scop::Window& window,
		RenderPass& render_pass
	);

	/* ========================================================================= */

	VkFormat							findDepthFormat(Device& device);

	/* ========================================================================= */

	VkSwapchainKHR						getSwapChain() const noexcept;
	VkFormat							getImageFormat() const noexcept;
	VkExtent2D							getExtent() const noexcept;
	const std::vector<VkFramebuffer>&	getFrameBuffers() const noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	VkSwapchainKHR						_swap_chain;
	VkFormat							_image_format;
	VkExtent2D							_extent;

	std::vector<VkImage>				_images;
	std::vector<VkImageView>			_image_views;
	std::vector<VkFramebuffer>			_frame_buffers;

	ImageBuffer							_color_image;
	ImageBuffer							_depth_image;

	/* ========================================================================= */

	void								_createSwapChain(
		Device& device,
		scop::Window& window
	);
	void								_createResources(Device& device);
	void								_createImages(
		Device& device,
		uint32_t& image_count
	);
	VkSurfaceFormatKHR					_chooseSwapSurfaceFormat(
		const std::vector<VkSurfaceFormatKHR>& available_formats
	) const noexcept;
	VkPresentModeKHR					_chooseSwapPresentMode(
		const std::vector<VkPresentModeKHR>& available_present_modes
	) const noexcept;
	VkExtent2D							_chooseSwapExtent(
		VkSurfaceCapabilitiesKHR capabilities,
		scop::Window& window
	) const;

}; // class SwapChain

} // namespace graphics
} // namespace scop