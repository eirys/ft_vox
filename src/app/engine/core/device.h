/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   device.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:14:30 by etran             #+#    #+#             */
/*   Updated: 2024/02/02 23:17:50 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# include <vulkan/vulkan.h>

// Std
# include <vector> // std::vector
# include <optional> // std::optional

namespace scop {
class Window;
} // namespace scop

namespace scop::core {

/* ========================================================================= */
/*                               HELPER OBJECTS                              */
/* ========================================================================= */

struct QueueFamilyIndices {
	std::optional<uint32_t>	graphics_family;
	std::optional<uint32_t>	present_family;
	std::optional<uint32_t>	compute_family;

	bool	isComplete() {
		return graphics_family.has_value() &&
			   present_family.has_value() &&
			   compute_family.has_value();
	}
};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR		capabilities;
	std::vector<VkSurfaceFormatKHR>	formats;
	std::vector<VkPresentModeKHR>	present_modes;
};

/**
 * @brief Wrapper class for Vulkan device.
*/
class Device {
public:
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
		scop::Window& window,
		VkInstance instance
	);
	void					destroy(VkInstance instance);
	void					idle();

	uint32_t				findMemoryType(
		uint32_t type_filter,
		VkMemoryPropertyFlags properties) const;
	SwapChainSupportDetails	querySwapChainSupport();
	QueueFamilyIndices		findQueueFamilies();
	VkFormat				findSupportedFormat(
		const std::vector<VkFormat>& candidates,
		VkImageTiling tiling,
		VkFormatFeatureFlags features) const;

	/* ========================================================================= */

	VkSurfaceKHR			getSurface() const noexcept;
	VkSampleCountFlagBits	getMsaaSamples() const noexcept;
	VkDevice				getLogicalDevice() const noexcept;
	VkPhysicalDevice		getPhysicalDevice() const noexcept;

	VkQueue					getGraphicsQueue() const noexcept;
	VkQueue					getPresentQueue() const noexcept;
	VkQueue					getComputeQueue() const noexcept;

private:
	/* ========================================================================= */
	/*                                HELPER CLASS                               */
	/* ========================================================================= */

	struct QueueFamilies {
		VkQueue				graphic;
		VkQueue				present;
		VkQueue				compute;
	};

	/* ========================================================================= */
	/*                               CONST MEMBERS                               */
	/* ========================================================================= */

	const std::vector<const char*>	_device_extensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	/* ========================================================================= */
	/*                                    DATA                                   */
	/* ========================================================================= */

	VkSurfaceKHR			_vk_surface;
	VkSampleCountFlagBits	_msaa_samples;

	VkPhysicalDevice		_physical_device = VK_NULL_HANDLE;
	VkDevice				_logical_device;

	QueueFamilies			_queue_families;

	std::vector<VkQueueFamilyProperties>	_queue_properties;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void					_createSurface(
		VkInstance instance,
		scop::Window& window);
	void					_pickPhysicalDevice(VkInstance vk_instance);
	void					_createLogicalDevice();

	VkSampleCountFlagBits	_getMaxUsableSampleCount() const;
	bool					_checkDeviceExtensionSupport(
		VkPhysicalDevice device);
	bool					_isDeviceSuitable(VkPhysicalDevice device);
	QueueFamilyIndices		_findQueueFamilies(VkPhysicalDevice device);
	SwapChainSupportDetails	_querySwapChainSupport(VkPhysicalDevice device);
	void					_retrieveQueueProperties(VkPhysicalDevice device);

}; // class Device

} // namespace scop::core