/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 11:53:00 by etran             #+#    #+#             */
/*   Updated: 2024/03/01 12:07:43 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.h"
#include "window.h"

#include <GLFW/glfw3.h>
#include <cstring>
#include <stdexcept>
#include <vector>

#include "debug.h"

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void Core::init(ui::Window& win) {
    _createInstance();
    m_debugModule.init(m_vkInstance);

    _createSurface(win);
    // _pickPhysicalDevice();
}

void Core::destroy() {
    vkDestroySurfaceKHR(m_vkInstance, m_vkSurface, nullptr);

    m_debugModule.destroy(m_vkInstance);
    vkDestroyInstance(m_vkInstance, nullptr);
}

/* ========================================================================== */

// const QueueFamilyIndices&  Core::getQueueFamilyIndices() const noexcept {
//     return m_queueFamilyIndices;
// }

// VkPhysicalDevice Core::getPhysicalDevice() const noexcept {
//     return m_vkPhysicalDevice;
// }

VkInstance Core::getInstance() const noexcept {
    return m_vkInstance;
}

VkSurfaceKHR Core::getSurface() const noexcept {
    return m_vkSurface;
}

// u32 Core::getMsaaCount() const noexcept {
//     return m_msaaCount;
// }

// /* ========================================================================== */

// u32 Core::findMemoryType(u32 typeFilter, VkMemoryPropertyFlags properties) const {
//     VkPhysicalDeviceMemoryProperties memProperties;
//     vkGetPhysicalDeviceMemoryProperties(m_vkPhysicalDevice, &memProperties);

//     for (u32 i = 0; i < memProperties.memoryTypeCount; ++i) {
//         const bool hasProperty = (memProperties.memoryTypes[i].propertyFlags & properties) == properties;
//         const bool isTypeFilter = typeFilter & (1 << i);
//         if (isTypeFilter && hasProperty)
//             return i;
//     }
//     throw std::runtime_error("failed to find suitable memory type");
// }

/* ========================================================================== */

// static
// SwapChainSupportDetails _querySwapChainSupport(VkPhysicalDevice physDevice, VkSurfaceKHR surface) {
//     SwapChainSupportDetails details;

//     vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physDevice, surface, &details.capabilities);

//     u32 formatCount;
//     vkGetPhysicalDeviceSurfaceFormatsKHR(physDevice, surface, &formatCount, nullptr);
//     if (formatCount != 0) {
//         details.formats.resize(formatCount);
//         vkGetPhysicalDeviceSurfaceFormatsKHR(physDevice, surface, &formatCount, details.formats.data());
//     }

//     u32 presentModeCount;
//     vkGetPhysicalDeviceSurfacePresentModesKHR(physDevice, surface, &presentModeCount, nullptr);
//     if (presentModeCount != 0) {
//         details.presentModes.resize(presentModeCount);
//         vkGetPhysicalDeviceSurfacePresentModesKHR(physDevice, surface, &presentModeCount, details.presentModes.data());
//     }

//     return details;
// }

// SwapChainSupportDetails Core::querySwapChainSupport() const {
//     return _querySwapChainSupport(m_vkPhysicalDevice, m_vkSurface);
// }

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

static
std::vector<const char*> _getRequiredExtensions() {
    u32             glfwExtensionCount = 0;
    const char**    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (DebugModule::ENABLE_VALIDATION_LAYERS)
        extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    return extensions;
}

static
bool	_hasValidationLayerSupport() {
	uint32_t	layer_count;
	vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

	std::vector<VkLayerProperties>	available_layers(layer_count);
	vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

	bool	supported = false;
	for (const char* layer_name: DebugModule::VALIDATION_LAYERS) {
		for (const VkLayerProperties& layer_properties: available_layers) {
			if (!strcmp(layer_name, layer_properties.layerName)) {
				supported = true;
				break;
			}
		}
	}
	return supported;
}

// static
// bool _hasValidationLayerSupport() {
//     u32 layerCount;
//     vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

//     std::vector<VkLayerProperties> availableLayers(layerCount);
//     vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

//     for (const char* layerName : DebugModule::VALIDATION_LAYERS) {
//         for (const VkLayerProperties& layerProperties: availableLayers) {
//             if (strcmp(layerName, layerProperties.layerName) == 0) {
//                 return true;
//             }
//         }
//     }
//     return false;
// }

void Core::_createInstance() {
    if (DebugModule::ENABLE_VALIDATION_LAYERS && !_hasValidationLayerSupport())
        throw std::runtime_error("Validation layers requested, but not available");

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Ft_vox";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Vox";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo instanceInfo{};
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pApplicationInfo = &appInfo;

    auto extensions = _getRequiredExtensions();
    instanceInfo.enabledExtensionCount = static_cast<u32>(extensions.size());
    instanceInfo.ppEnabledExtensionNames = extensions.data();

    if (DebugModule::ENABLE_VALIDATION_LAYERS) {
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = DebugModule::getMessengerCreateInfo();
        instanceInfo.enabledLayerCount = static_cast<u32>(DebugModule::VALIDATION_LAYERS.size());
        instanceInfo.ppEnabledLayerNames = DebugModule::VALIDATION_LAYERS.data();
        instanceInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    } else {
        instanceInfo.enabledExtensionCount = 0;
        instanceInfo.pNext = nullptr;
    }

    LDEBUG("Creating");
    if (vkCreateInstance(&instanceInfo, nullptr, &m_vkInstance) != VK_SUCCESS)
        throw std::runtime_error("Failed to create instance");
    LDEBUG("Vulkan instance created");
}

/* ========================================================================== */

void Core::_createSurface(ui::Window& win) {
    if (glfwCreateWindowSurface(m_vkInstance, win.getWindow(), nullptr, &m_vkSurface) != VK_SUCCESS)
        throw std::runtime_error("Failed to create window surface");
}

/* ========================================================================== */

// /**
//  * @brief Get the maximum sample count supported by the physical device.
// */
// static
// VkSampleCountFlagBits _getMaxUsableSampleCount(VkPhysicalDevice physDevice) {
//     VkPhysicalDeviceProperties physicalDeviceProperties;
//     vkGetPhysicalDeviceProperties(physDevice, &physicalDeviceProperties);

//     VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts &
//                                 physicalDeviceProperties.limits.framebufferDepthSampleCounts;

//     constexpr VkSampleCountFlagBits countsArray[] = {
//         VK_SAMPLE_COUNT_64_BIT,
//         VK_SAMPLE_COUNT_32_BIT,
//         VK_SAMPLE_COUNT_16_BIT,
//         VK_SAMPLE_COUNT_8_BIT,
//         VK_SAMPLE_COUNT_4_BIT,
//         VK_SAMPLE_COUNT_2_BIT };

//     for (VkSampleCountFlagBits count: countsArray) {
//         if (counts & count)
//             return count;
//     }
//     return VK_SAMPLE_COUNT_1_BIT;
// }

// void Core::_pickPhysicalDevice() {
//     u32 physDeviceCount = 0;
//     vkEnumeratePhysicalDevices(m_vkInstance, &physDeviceCount, nullptr);

//     if (physDeviceCount == 0)
//         throw std::runtime_error("Failed to find GPUs with Vulkan support");

//     std::vector<VkPhysicalDevice> physDevices(physDeviceCount);
//     vkEnumeratePhysicalDevices(m_vkInstance, &physDeviceCount, physDevices.data());

//     for (const VkPhysicalDevice& physDevice : physDevices) {
//         m_queueFamilyIndices = _findQueueFamilies(physDevice);
//         if (_isDeviceSuitable(physDevice)) {
//             m_vkPhysicalDevice = physDevice;
//             m_msaaCount = _getMaxUsableSampleCount(physDevice);
//             return;
//         }
//     }
//     throw std::runtime_error("Failed to find a suitable GPU");
// }

/* ========================================================================== */

// static
// std::vector<VkQueueFamilyProperties> _retrieveQueueProperties(VkPhysicalDevice physDevice) {
//     u32 queueFamilyCount = 0;
//     vkGetPhysicalDeviceQueueFamilyProperties(physDevice, &queueFamilyCount, nullptr);
//     std::vector<VkQueueFamilyProperties> queueProperties(queueFamilyCount);
//     vkGetPhysicalDeviceQueueFamilyProperties(physDevice, &queueFamilyCount, queueProperties.data());
//     return queueProperties;
// }

// QueueFamilyIndices Core::_findQueueFamilies(VkPhysicalDevice physDevice) const {
//     QueueFamilyIndices indices;
//     std::vector<VkQueueFamilyProperties> queueProperties = _retrieveQueueProperties(physDevice);

//     for (u32 i = 0; i < queueProperties.size() && !indices.isComplete(); ++i) {
//         const VkQueueFamilyProperties& queueFamily = queueProperties[i];

//         // Graphics
//         if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
//             indices.graphicsFamily = i;

//         // Compute
//         if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
//             indices.computeFamily = i;

//         // Present
//         VkBool32 presentSupport = false;
//         vkGetPhysicalDeviceSurfaceSupportKHR(physDevice, i, m_vkSurface, &presentSupport);
//         if (presentSupport)
//             indices.presentFamily = i;
//     }
//     return indices;
// }

/* ========================================================================== */

// /**
//  * @brief Check if the device supports the required extensions.
//  */
// static
// bool _hasExpectedExtensions(VkPhysicalDevice physDevice) {
//     u32 extensionCount;
//     vkEnumerateDeviceExtensionProperties(physDevice, nullptr, &extensionCount, nullptr);

//     std::vector<VkExtensionProperties> availableExtensions(extensionCount);
//     vkEnumerateDeviceExtensionProperties(
//         physDevice,
//         nullptr,
//         &extensionCount,
//         availableExtensions.data());

//     std::set<std::string> requiredExtensions(Core::DEVICE_EXTENSIONS.begin(), Core::DEVICE_EXTENSIONS.end());
//     for (const VkExtensionProperties& extension: availableExtensions)
//         requiredExtensions.erase(extension.extensionName);

//     return requiredExtensions.empty();
// }

// static
// bool _hasExpectedFeatures(const VkPhysicalDevice physDevice) {
//     VkPhysicalDeviceFeatures supportedFeatures;
//     vkGetPhysicalDeviceFeatures(physDevice, &supportedFeatures);

//     return supportedFeatures.samplerAnisotropy;
// }

// static
// bool _hasExpectedFormatProperties(const VkPhysicalDevice physDevice) {
//     // Dynamic blitting support
//     for (VkFormat format: BLITTED_IMAGE_FORMATS) {
//         VkFormatProperties	properties;
//         vkGetPhysicalDeviceFormatProperties(physDevice, format, &properties);
//         const bool hasLinearTiling = (bool)(properties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT);

//         if (!hasLinearTiling) return false;
//     }
//     return true;
// }

// /**
//  * @brief Check if the physical device is suitable for the application,
//  * given the requirements.
//  * @param physDevice the physical device to check.
//  */
// bool Core::_isDeviceSuitable(const VkPhysicalDevice physDevice) const {
//     const bool  extensionsSupported = _hasExpectedExtensions(physDevice);
//     bool        isSwapChainAdequate = false;

//     if (extensionsSupported) {
//         SwapChainSupportDetails swapChainSupport = _querySwapChainSupport(physDevice, m_vkSurface);
//         isSwapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
//     }

//     return m_queueFamilyIndices.isComplete() &&
//            extensionsSupported &&
//            isSwapChainAdequate &&
//            _hasExpectedFormatProperties(physDevice) &&
//            _hasExpectedFeatures(physDevice);
// }

} // namespace vox::gfx
