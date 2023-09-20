/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   device.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:14:30 by etran             #+#    #+#             */
/*   Updated: 2023/08/12 23:43:18 by etran            ###   ########.fr       */
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
# include <optional> // std::optional

namespace scop {
class Window;
} // namespace scop

namespace scop::graphics {

/**
 * @brief Wrapper class for Vulkan device.
*/
class Device {
public:
	/* ========================================================================= */
	/*                               HELPER OBJECTS                              */
	/* ========================================================================= */

	struct QueueFamilyIndices {
		std::optional<uint32_t>	graphics_family;
		std::optional<uint32_t>	present_family;

		bool	isComplete() {
			return graphics_family.has_value() && present_family.has_value();
		}
	};

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR		capabilities;
		std::vector<VkSurfaceFormatKHR>	formats;
		std::vector<VkPresentModeKHR>	present_modes;
	};

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	Device() = default;
	~Device() = default;

	Device(Device&& other) = delete;
	Device(const Device& other) = delete;
	Device& operator=(Device&& other) = delete;
	Device& operator=(const Device& other) = delete;

	/* ========================================================================= */

	void					init(
		::scop::Window& window,
		VkInstance instance
	);
	void					destroy(VkInstance instance);
	void					idle();

	uint32_t				findMemoryType(
		uint32_t type_filter,
		VkMemoryPropertyFlags properties
	) const;
	SwapChainSupportDetails	querySwapChainSupport();
	QueueFamilyIndices		findQueueFamilies();
	VkFormat				findSupportedFormat(
		const std::vector<VkFormat>& candidates,
		VkImageTiling tiling,
		VkFormatFeatureFlags features
	);

	/* ========================================================================= */

	VkSurfaceKHR			getSurface() const noexcept;
	VkSampleCountFlagBits	getMsaaSamples() const noexcept;
	VkDevice				getLogicalDevice() const noexcept;
	VkPhysicalDevice		getPhysicalDevice() const noexcept;
	VkQueue					getGraphicsQueue() const noexcept;
	VkQueue					getPresentQueue() const noexcept;

private:
	/* ========================================================================= */
	/*                               CONST MEMBERS                               */
	/* ========================================================================= */

	const std::vector<const char*>	_device_extensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	VkSurfaceKHR			_vk_surface;
	VkSampleCountFlagBits	_msaa_samples;

	VkPhysicalDevice		_physical_device = VK_NULL_HANDLE;
	VkDevice				_logical_device;

	VkQueue					_graphics_queue;
	VkQueue					_present_queue;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void					_createSurface(
		VkInstance instance,
		::scop::Window& window
	);
	void					_pickPhysicalDevice(VkInstance vk_instance);
	void					_createLogicalDevice();

	VkSampleCountFlagBits	_getMaxUsableSampleCount() const;
	bool					_checkDeviceExtensionSupport(
		VkPhysicalDevice device
	);
	bool					_isDeviceSuitable(VkPhysicalDevice device);
	QueueFamilyIndices		_findQueueFamilies(VkPhysicalDevice device);
	SwapChainSupportDetails	_querySwapChainSupport(VkPhysicalDevice device);

}; // class Device

} // namespace scop::graphics