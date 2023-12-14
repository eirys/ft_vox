/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap_chain.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/02 19:40:11 by etran             #+#    #+#             */
/*   Updated: 2023/12/04 23:20:47 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "swap_chain.h"
#include "window.h"
#include "device.h"

#include <algorithm> // std::clamp
#include <stdexcept> // std::runtime_error

namespace scop {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void	SwapChain::init(
	core::Device& device,
	Window& window
) {
	_createSwapChain(device, window);
}

void	SwapChain::destroy(core::Device& device) {
	for (std::size_t i = 0; i < _image_views.size(); ++i) {
		vkDestroyImageView(
			device.getLogicalDevice(),
			_image_views[i],
			nullptr);
	}
	// Remove swap chain handler
	vkDestroySwapchainKHR(device.getLogicalDevice(), _swap_chain, nullptr);
}

void	SwapChain::update(
	core::Device& device,
	Window& window
) {
	window.pause();
	device.idle();

	destroy(device);
	init(device, window);
}

/* ========================================================================== */

/**
 * @brief Find a depth format for the wanted features.
*/
VkFormat	SwapChain::findDepthFormat(const core::Device& device) const {
	return device.findSupportedFormat({
			VK_FORMAT_D32_SFLOAT,
			VK_FORMAT_D32_SFLOAT_S8_UINT,
			VK_FORMAT_D24_UNORM_S8_UINT
		}, VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);
}

/* ========================================================================== */

VkSwapchainKHR	SwapChain::getSwapChain() const noexcept {
	return _swap_chain;
}

VkFormat	SwapChain::getImageFormat() const noexcept {
	return _image_format;
}

VkExtent2D	SwapChain::getExtent() const noexcept {
	return _extent;
}

const std::vector<VkImage>&	SwapChain::getImages() const noexcept {
	return _images;
}

const std::vector<VkImageView>&	SwapChain::getImageViews() const noexcept {
	return _image_views;
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

void	SwapChain::_createSwapChain(
	core::Device& device,
	Window& window
) {
	VkSwapchainKHR	old_swap_chain = _swap_chain;

	core::SwapChainSupportDetails	swap_chain_support =
		device.querySwapChainSupport();

	// Setup options for functionning swap chain
	VkSurfaceFormatKHR	surface_format = _chooseSwapSurfaceFormat(
		swap_chain_support.formats);
	VkPresentModeKHR	present_mode = _chooseSwapPresentMode(
		swap_chain_support.present_modes);
	VkExtent2D			swap_extent = _chooseSwapExtent(
		swap_chain_support.capabilities,
		window);

	// Nb of images in the swap chain
	uint32_t	image_count =
		swap_chain_support.capabilities.minImageCount + 1;

	if (swap_chain_support.capabilities.maxImageCount > 0 &&
		image_count > swap_chain_support.capabilities.maxImageCount) {
		// Avoid value exceeding max
		image_count = swap_chain_support.capabilities.maxImageCount;
	}

	// Setup the creation of the swap chain object, tied to the vk_surface
	VkSwapchainCreateInfoKHR	swapchain_info{};
	swapchain_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchain_info.surface = device.getSurface();
	swapchain_info.minImageCount = image_count;
	swapchain_info.imageFormat = surface_format.format;
	swapchain_info.imageColorSpace = surface_format.colorSpace;
	swapchain_info.imageExtent = swap_extent;
	swapchain_info.imageArrayLayers = 1;
	swapchain_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	// Queue family swap handling:
	// - gfx queue -> drawing to swap chain
	// - present queue -> get passed the swap chain to be submitted
	core::QueueFamilyIndices	indices = device.findQueueFamilies();
	uint32_t			queue_family_indices[] = {
		indices.graphics_family.value(),
		indices.present_family.value() };

	if (indices.graphics_family != indices.present_family) {
		// No image ownership transfer
		swapchain_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swapchain_info.queueFamilyIndexCount = 2;
		swapchain_info.pQueueFamilyIndices = queue_family_indices;
	} else {
		// Explicit image ownership needed between queue families
		swapchain_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchain_info.queueFamilyIndexCount = 0;
		swapchain_info.pQueueFamilyIndices = nullptr;
	}

	swapchain_info.preTransform = swap_chain_support.capabilities.currentTransform;
	swapchain_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchain_info.presentMode = present_mode;
	swapchain_info.clipped = VK_TRUE;
	swapchain_info.oldSwapchain = old_swap_chain;

	// Create the object and images
	if (vkCreateSwapchainKHR(device.getLogicalDevice(), &swapchain_info, nullptr, &_swap_chain) != VK_SUCCESS) {
		throw std::runtime_error("failed to create swap chain");
	}

	// Remove old swap chain handler
	if (old_swap_chain != VK_NULL_HANDLE) {
		vkDestroySwapchainKHR(device.getLogicalDevice(), old_swap_chain, nullptr);
	}

	_image_format = surface_format.format;
	_extent = swap_extent;
	_createImages(device, image_count);
}

/**
 * @brief Retrieve VkImage handles from the swap chain and create image views.
*/
void	SwapChain::_createImages(core::Device& device, uint32_t& image_count) {
	// Retrieve VkImage handles
	vkGetSwapchainImagesKHR(
		device.getLogicalDevice(),
		_swap_chain,
		&image_count, nullptr
	);
	_images.resize(image_count);
	vkGetSwapchainImagesKHR(
		device.getLogicalDevice(),
		_swap_chain,
		&image_count, _images.data()
	);

	// Create image view for each image
	_image_views.reserve(image_count);
	auto createImageViewFunc =
		[&device, this]
		(VkImage image) -> VkImageView {
			VkImageViewCreateInfo	view{};
			view.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			view.image = image;
			view.viewType = VK_IMAGE_VIEW_TYPE_2D;
			view.format = _image_format;
			view.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			view.subresourceRange.levelCount = 1;
			view.subresourceRange.layerCount = 1;

			VkImageView	image_view;
			if (vkCreateImageView(device.getLogicalDevice(), &view, nullptr, &image_view) != VK_SUCCESS) {
				throw std::runtime_error("failed to create image views");
			}
			return image_view;
		};

	for (std::size_t i = 0; i < image_count; ++i) {
		_image_views.emplace_back(createImageViewFunc(_images[i]));
	}
}

/**
 * @brief Check if surface format is adequate.
*/
VkSurfaceFormatKHR	SwapChain::_chooseSwapSurfaceFormat(
	const std::vector<VkSurfaceFormatKHR>& available_formats
) const noexcept {
	// Prevalent: sRGB format for non linear color representation
	for (const auto& available_format: available_formats) {
		if (available_format.format == VK_FORMAT_B8G8R8A8_SRGB
		&& available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			return available_format;
	}
	// Default case (no wanted format)
	return available_formats[0];
}

/**
 * @brief Check if present mode is adequate.
*/
VkPresentModeKHR	SwapChain::_chooseSwapPresentMode(
	const std::vector<VkPresentModeKHR>& available_present_modes
) const noexcept {
	// Prevalent: fifo mailbox mode, expansive on energy but efficient
	for (const auto& available_present_mode: available_present_modes) {
		if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return available_present_mode;
		}
	}
	// Default case, doesn't strain on battery
	return VK_PRESENT_MODE_FIFO_KHR;
}

/**
 * @brief Check if swap extent is adequate with the window size.
*/
VkExtent2D	SwapChain::_chooseSwapExtent(
	VkSurfaceCapabilitiesKHR capabilities,
	Window& window
) const {
	// Pick swap extent (~ resolution of the window, in px)
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	} else {
		int	width, height;
		window.retrieveSize(width, height);

		VkExtent2D	actual_extent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		actual_extent.width = std::clamp(
			actual_extent.width,
			capabilities.minImageExtent.width,
			capabilities.maxImageExtent.width
		);
		actual_extent.height = std::clamp(
			actual_extent.height,
			capabilities.minImageExtent.height,
			capabilities.maxImageExtent.height
		);
		return actual_extent;
	}
}

} // namespace scop