/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap_chain.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/02 16:27:11 by etran             #+#    #+#             */
/*   Updated: 2023/11/16 23:07:58 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# include <vulkan/vulkan.h>

// Std
# include <vector> // std::vector

namespace scop::core {
class Device;
}

namespace scop::gfx {
class RenderPass;
}

namespace scop {

class Window;

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
		core::Device& device,
		Window& window);
	void								destroy(core::Device& device);
	void								update(
		core::Device& device,
		Window& window);

	/* ========================================================================= */

	VkFormat							findDepthFormat(core::Device& device);

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
		core::Device& device,
		Window& window);
	void								_createImages(
		core::Device& device,
		uint32_t& image_count);

	VkSurfaceFormatKHR					_chooseSwapSurfaceFormat(
		const std::vector<VkSurfaceFormatKHR>& available_formats) const noexcept;
	VkPresentModeKHR					_chooseSwapPresentMode(
		const std::vector<VkPresentModeKHR>& available_present_modes) const noexcept;
	VkExtent2D							_chooseSwapExtent(
		VkSurfaceCapabilitiesKHR capabilities,
		Window& window) const;

}; // class SwapChain

} // namespace scop