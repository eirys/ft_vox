/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   device.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 01:00:19 by etran             #+#    #+#             */
/*   Updated: 2023/08/10 22:14:49 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "device.h"
#include "debug_module.h"
#include "window.h"
#include "utils.h"
#include "buffer.h"

#include <vector> // std::vector
#include <set> // std::set
#include <stdexcept> // std::runtime_error

namespace scop::graphics {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void	Device::init(::scop::Window& window, VkInstance instance) {
	_createSurface(instance, window);
	_pickPhysicalDevice(instance);
	_createLogicalDevice();
}

void	Device::destroy(VkInstance instance) {
	vkDestroyDevice(_logical_device, nullptr);
	vkDestroySurfaceKHR(instance, _vk_surface, nullptr);
}

void	Device::idle() {
	vkDeviceWaitIdle(_logical_device);
}

/**
 * @brief Map memory and find one suitable with filter and properties.
*/
uint32_t	Device::findMemoryType(
	uint32_t type_filter,
	VkMemoryPropertyFlags properties
) const {
	VkPhysicalDeviceMemoryProperties	mem_properties;
	vkGetPhysicalDeviceMemoryProperties(_physical_device, &mem_properties);

	for (uint32_t i = 0; i < mem_properties.memoryTypeCount; ++i) {
		if ((mem_properties.memoryTypes[i].propertyFlags & properties) == properties &&
			(type_filter & (1 << i))) {
			return i;
		}
	}
	throw std::runtime_error("failed to find suitable memory type");
}

/**
 * @brief Retrieve queue families that are appropriate for
 * the physical device and the app needs.
*/
Device::QueueFamilyIndices	Device::findQueueFamilies() {
	return _findQueueFamilies(_physical_device);
}

/**
 * @brief Checks if the physical device is suitable for the app.
*/
Device::SwapChainSupportDetails	Device::querySwapChainSupport() {
	return _querySwapChainSupport(_physical_device);
}

/**
 * @brief Find best suited format for depth image
*/
VkFormat	Device::findSupportedFormat(
	const std::vector<VkFormat>& candidates,
	VkImageTiling tiling,
	VkFormatFeatureFlags features
) {
	for (VkFormat format: candidates) {
		// Query format properties for candidate
		VkFormatProperties	properties;
		vkGetPhysicalDeviceFormatProperties(
			_physical_device,
			format,
			&properties
		);

		if ((
			tiling == VK_IMAGE_TILING_LINEAR
			&& (properties.linearTilingFeatures & features) == features
		) || (
			tiling == VK_IMAGE_TILING_OPTIMAL
			&& (properties.optimalTilingFeatures & features) == features
		)) {
			return format;
		}
	}
	throw std::runtime_error("failed to find supported format");
}

/* ========================================================================== */

VkSurfaceKHR	Device::getSurface() const noexcept {
	return _vk_surface;
}

VkSampleCountFlagBits	Device::getMsaaSamples() const noexcept {
	return _msaa_samples;
}

VkDevice	Device::getLogicalDevice() const noexcept {
	return _logical_device;
}

VkPhysicalDevice	Device::getPhysicalDevice() const noexcept {
	return _physical_device;
}

VkQueue	Device::getGraphicsQueue() const noexcept {
	return _graphics_queue;
}

VkQueue	Device::getPresentQueue() const noexcept {
	return _present_queue;
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

/**
 * @brief Establish connection between Vulkan instance and the window system.
*/
void	Device::_createSurface(
	VkInstance instance,
	::scop::Window& window
) {
	if (glfwCreateWindowSurface(instance, window.getWindow(), nullptr, &_vk_surface) != VK_SUCCESS)
		throw std::runtime_error("failed to create window surface");
}

/**
 * Pick a physical device (GPU) to use
*/
void	Device::_pickPhysicalDevice(VkInstance instance) {
	uint32_t	device_count = 0;
	vkEnumeratePhysicalDevices(instance, &device_count, nullptr);

	if (device_count == 0)
		throw std::runtime_error("failed to find GPUs with vulkan support");

	std::vector<VkPhysicalDevice>	devices(device_count);
	vkEnumeratePhysicalDevices(instance, &device_count, devices.data());

	for (const VkPhysicalDevice& device: devices) {
		if (_isDeviceSuitable(device)) {
			_physical_device = device;
			_msaa_samples = _getMaxUsableSampleCount();
			break;
		}
	}

	if (_physical_device == VK_NULL_HANDLE)
		throw std::runtime_error("failed to find suitable GPU");
}

/**
 * Setup logical device, the interface between the app and the physical device (GPU).
*/
void	Device::_createLogicalDevice() {
	// Indicate that we want to create a single queue, with graphics capabilities
	QueueFamilyIndices	indices = findQueueFamilies();

	std::vector<VkDeviceQueueCreateInfo>	queue_create_infos;
	std::set<uint32_t>						unique_queue_families = {
		indices.graphics_family.value(),
		indices.present_family.value()
	};

	float	queue_priority = 1.0f;

	for (uint32_t queue_family: unique_queue_families) {
		VkDeviceQueueCreateInfo	queue_create_info{};

		queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queue_create_info.queueFamilyIndex = queue_family;
		queue_create_info.queueCount = 1;
		queue_create_info.pQueuePriorities = &queue_priority;
		queue_create_infos.emplace_back(queue_create_info);
	}

	// Enable device features: anisotropic filtering and cube map array
	VkPhysicalDeviceFeatures	device_features{};
	device_features.samplerAnisotropy = VK_TRUE;
	// device_features.imageCubeArray = VK_TRUE;

	VkDeviceCreateInfo			create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	create_info.pQueueCreateInfos = queue_create_infos.data();
	create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
	create_info.pEnabledFeatures = &device_features;

	// Validation layers
	if (DebugModule::enable_validation_layers) {
		create_info.enabledLayerCount = static_cast<uint32_t>(
			DebugModule::validation_layers.size()
		);
		create_info.ppEnabledLayerNames = DebugModule::validation_layers.data();
	} else {
		create_info.enabledLayerCount = 0;
	}

	// Device extensions enabling, notably for swap chain support
	create_info.enabledExtensionCount = static_cast<uint32_t>(_device_extensions.size());
	create_info.ppEnabledExtensionNames = _device_extensions.data();

	if (vkCreateDevice(_physical_device, &create_info, nullptr, &_logical_device) != VK_SUCCESS)
		throw std::runtime_error("failed to create logical device");

	// Retrieve queue handles
	vkGetDeviceQueue(
		_logical_device,
		indices.graphics_family.value(),
		0,
		&_graphics_queue
	);
	vkGetDeviceQueue(
		_logical_device,
		indices.present_family.value(),
		0,
		&_present_queue
	);
}

/**
 * Retrieve max sample count for MSAA (multisample antialiasing)
*/
VkSampleCountFlagBits	Device::_getMaxUsableSampleCount() const {
	VkPhysicalDeviceProperties	properties;
	vkGetPhysicalDeviceProperties(_physical_device, &properties);

	VkSampleCountFlags	count =
		properties.limits.framebufferColorSampleCounts &
		properties.limits.framebufferDepthSampleCounts;

	VkSampleCountFlagBits	values[] = {
		VK_SAMPLE_COUNT_64_BIT,
		VK_SAMPLE_COUNT_32_BIT,
		VK_SAMPLE_COUNT_16_BIT,
		VK_SAMPLE_COUNT_8_BIT,
		VK_SAMPLE_COUNT_4_BIT,
		VK_SAMPLE_COUNT_2_BIT
	};

	for (VkSampleCountFlagBits value : values) {
		if (count & value) {
			return value;
		}
	}
	return VK_SAMPLE_COUNT_1_BIT;
}

/**
 * Check if all required extensions are available for the physical device
*/
bool	Device::_checkDeviceExtensionSupport(VkPhysicalDevice device) {
	// Verify that every device_extensions are available
	uint32_t	extension_count;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);

	std::vector<VkExtensionProperties>	available_extensions(extension_count);
	vkEnumerateDeviceExtensionProperties(
		device,
		nullptr,
		&extension_count,
		available_extensions.data()
	);

	std::set<std::string>	required_extensions(
		_device_extensions.begin(),
		_device_extensions.end()
	);
	for (const auto& extension: available_extensions) {
		required_extensions.erase(extension.extensionName);
	}

	return required_extensions.empty();
}

/**
 * Verify that the selected physical device is suitable for the app needs
*/
bool	Device::_isDeviceSuitable(VkPhysicalDevice device) {
	QueueFamilyIndices	indices = _findQueueFamilies(device);
	bool	extensions_supported = _checkDeviceExtensionSupport(device);
	bool	swap_chain_adequate = false;

	if (extensions_supported) {
		SwapChainSupportDetails	swap_chain_support =
			_querySwapChainSupport(device);
		swap_chain_adequate =
			!swap_chain_support.formats.empty() &&
			!swap_chain_support.present_modes.empty();
	}

	VkPhysicalDeviceFeatures	supported_features;
	vkGetPhysicalDeviceFeatures(device, &supported_features);

	return (
		indices.isComplete() &&
		extensions_supported &&
		swap_chain_adequate &&
		supported_features.samplerAnisotropy
	);
}

/**
 * @brief Make sure the swap chain support is available for this device.
*/
Device::SwapChainSupportDetails	Device::_querySwapChainSupport(
	VkPhysicalDevice device
) {
	SwapChainSupportDetails	details;

	// Query supported surface formats
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
		device,
		_vk_surface,
		&details.capabilities
	);
	uint32_t	format_count;
	vkGetPhysicalDeviceSurfaceFormatsKHR(
		device,
		_vk_surface,
		&format_count, nullptr
	);
	if (format_count != 0) {
		details.formats.resize(format_count);
		vkGetPhysicalDeviceSurfaceFormatsKHR(
			device,
			_vk_surface,
			&format_count, details.formats.data()
		);
	}

	// Query supported presentation modes
	uint32_t	present_mode_count;
	vkGetPhysicalDeviceSurfacePresentModesKHR(
		device,
		_vk_surface,
		&present_mode_count, nullptr
	);
	if (present_mode_count != 0) {
		details.present_modes.resize(present_mode_count);
		vkGetPhysicalDeviceSurfacePresentModesKHR(
			device, _vk_surface,
			&present_mode_count,
			details.present_modes.data()
		);
	}

	return details;
}

/**
 * @brief Retrieve queue families that are appropriate for
 * the selected physical device and the app needs.
*/
Device::QueueFamilyIndices	Device::_findQueueFamilies(
	VkPhysicalDevice device
) {
	QueueFamilyIndices	indices;
	uint32_t			queue_family_count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(
		device,
		&queue_family_count, nullptr
	);
	std::vector<VkQueueFamilyProperties>	queue_families(queue_family_count);
	vkGetPhysicalDeviceQueueFamilyProperties(
		device,
		&queue_family_count, queue_families.data()
	);

	int	i = 0;
	for (const auto& queue_family: queue_families) {
		// Looking for queue family that supports the graphics bit flag
		if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			indices.graphics_family = i;

		// Looking for queue family that supports presenting
		VkBool32	present_support = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(
			device,
			i,
			_vk_surface,
			&present_support
		);
		if (present_support)
			indices.present_family = i;
		if (indices.isComplete())
			break;
		++i;
	}

	return indices;
}

} // namespace scop::graphics