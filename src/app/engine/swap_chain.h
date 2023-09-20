/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap_chain.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/02 16:27:11 by etran             #+#    #+#             */
/*   Updated: 2023/08/11 23:14:00 by etran            ###   ########.fr       */
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
class Window;
} // namespace scop

namespace scop::graphics {

class Device;
class RenderPass;

/**
 * @brief Wrapper class for swap chain.
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
		::scop::Window& window);
	void								destroy(Device& device);
	void								update(
		Device& device,
		::scop::Window& window);

	/* ========================================================================= */

	VkFormat							findDepthFormat(Device& device);

	/* ========================================================================= */

	VkSwapchainKHR						getSwapChain() const noexcept;
	VkFormat							getImageFormat() const noexcept;
	VkExtent2D							getExtent() const noexcept;

	const std::vector<VkImage>&			getImages() const noexcept;
	const std::vector<VkImageView>&		getImageViews() const noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	VkSwapchainKHR						_swap_chain;
	VkFormat							_image_format;
	VkExtent2D							_extent;

	std::vector<VkImage>				_images;
	std::vector<VkImageView>			_image_views;

	/* ========================================================================= */

	void								_createSwapChain(
		Device& device,
		::scop::Window& window);
	void								_createImages(
		Device& device,
		uint32_t& image_count);

	VkSurfaceFormatKHR					_chooseSwapSurfaceFormat(
		const std::vector<VkSurfaceFormatKHR>& available_formats) const noexcept;
	VkPresentModeKHR					_chooseSwapPresentMode(
		const std::vector<VkPresentModeKHR>& available_present_modes) const noexcept;
	VkExtent2D							_chooseSwapExtent(
		VkSurfaceCapabilitiesKHR capabilities,
		::scop::Window& window) const;

}; // class SwapChain

} // namespace scop::graphics