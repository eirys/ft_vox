/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   device.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 23:37:02 by etran             #+#    #+#             */
/*   Updated: 2024/03/07 12:31:54 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "device.h"
#include "core.h"
#include "types.h"
#include "buffer_decl.h"
#include "swap_chain.h"

#include <set>
#include <array>
#include <stdexcept>

namespace vox::gfx {

/* ========================================================================== */
/*                               STATIC MEMBERS                               */
/* ========================================================================== */

static constexpr
std::array<const char*, 1>  DEVICE_EXTENSIONS = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void Device::init(const Core& core) {
    _pickPhysicalDevice(core);
    _createLogicalDevice();
}

void Device::destroy() {
    vkDestroyDevice(m_vkLogicalDevice, nullptr);
}

void Device::idle() const {
    vkDeviceWaitIdle(m_vkLogicalDevice);
}

/* ========================================================================== */

static
SwapChainSupportDetails _querySwapChainSupport(const VkPhysicalDevice physDevice, const VkSurfaceKHR surface) {
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physDevice, surface, &details.m_capabilities);

    u32 formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physDevice, surface, &formatCount, nullptr);
    if (formatCount != 0) {
        details.m_formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physDevice, surface, &formatCount, details.m_formats.data());
    }

    u32 presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physDevice, surface, &presentModeCount, nullptr);
    if (presentModeCount != 0) {
        details.m_presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physDevice, surface, &presentModeCount, details.m_presentModes.data());
    }

    return details;
}

SwapChainSupportDetails Device::querySwapChainSupport(const Core& core) const {
    return _querySwapChainSupport(m_vkPhysicalDevice, core.getSurface());
}

/* ========================================================================== */

u32 Device::findMemoryType(const u32 typeFilter, const VkMemoryPropertyFlags properties) const {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(m_vkPhysicalDevice, &memProperties);

    for (u32 i = 0; i < memProperties.memoryTypeCount; ++i) {
        const bool hasProperty = (memProperties.memoryTypes[i].propertyFlags & properties) == properties;
        const bool isTypeFilter = typeFilter & (1 << i);
        if (isTypeFilter && hasProperty)
            return i;
    }
    throw std::runtime_error("failed to find suitable memory type");
}

/* ========================================================================== */

/* ========================================================================== */

VkDevice Device::getDevice() const noexcept {
    return m_vkLogicalDevice;
}

VkQueue Device::getGraphicsQueue() const noexcept {
    return m_queueFamilies.m_graphics;
}

VkQueue Device::getPresentQueue() const noexcept {
    return m_queueFamilies.m_present;
}

VkQueue Device::getComputeQueue() const noexcept {
    return m_queueFamilies.m_compute;
}

VkSampleCountFlagBits Device::getMsaaCount() const noexcept {
    return m_msaaCount;
}

const QueueFamilyIndices&  Device::getQueueFamilyIndices() const noexcept {
    return m_queueFamilyIndices;
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

static
std::vector<VkQueueFamilyProperties> _retrieveQueueProperties(const VkPhysicalDevice physDevice) {
    u32 queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physDevice, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueProperties(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physDevice, &queueFamilyCount, queueProperties.data());
    return queueProperties;
}

static
QueueFamilyIndices _findQueueFamilies(const VkSurfaceKHR surface, const VkPhysicalDevice physDevice) {
    const std::vector<VkQueueFamilyProperties> queueProperties = _retrieveQueueProperties(physDevice);

    QueueFamilyIndices indices;

    for (u32 i = 0; i < queueProperties.size() && !indices.isComplete(); ++i) {
        const VkQueueFamilyProperties& queueFamily = queueProperties[i];

        // Graphics
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            indices.m_graphicsFamily = i;

        // Compute
        if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
            indices.m_computeFamily = i;

        // Present
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physDevice, i, surface, &presentSupport);
        if (presentSupport)
            indices.m_presentFamily = i;
    }
    return indices;
}

/**
 * @brief Get the maximum sample count supported by the physical device.
*/
static
VkSampleCountFlagBits _getMaxUsableSampleCount(VkPhysicalDevice physDevice) {
    VkPhysicalDeviceProperties physicalDeviceProperties;
    vkGetPhysicalDeviceProperties(physDevice, &physicalDeviceProperties);

    VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts &
                                physicalDeviceProperties.limits.framebufferDepthSampleCounts;

    constexpr VkSampleCountFlagBits countsArray[] = {
        VK_SAMPLE_COUNT_64_BIT,
        VK_SAMPLE_COUNT_32_BIT,
        VK_SAMPLE_COUNT_16_BIT,
        VK_SAMPLE_COUNT_8_BIT,
        VK_SAMPLE_COUNT_4_BIT,
        VK_SAMPLE_COUNT_2_BIT };

    for (VkSampleCountFlagBits count: countsArray) {
        if (counts & count)
            return count;
    }
    return VK_SAMPLE_COUNT_1_BIT;
}

/**
 * @brief Check if the device supports the required extensions.
 */
static
bool _hasExpectedExtensions(const VkPhysicalDevice physDevice) {
    u32 extensionCount;
    vkEnumerateDeviceExtensionProperties(physDevice, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(
        physDevice,
        nullptr,
        &extensionCount,
        availableExtensions.data());

    std::set<std::string> requiredExtensions(DEVICE_EXTENSIONS.begin(), DEVICE_EXTENSIONS.end());
    for (const VkExtensionProperties& extension: availableExtensions)
        requiredExtensions.erase(extension.extensionName);

    return requiredExtensions.empty();
}

static
bool _hasExpectedFeatures(const VkPhysicalDevice physDevice) {
    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(physDevice, &supportedFeatures);

    return supportedFeatures.samplerAnisotropy;
}

static
bool _hasExpectedFormatProperties(const VkPhysicalDevice physDevice) {
    // Dynamic blitting support
    for (const VkFormat format: BLITTED_IMAGE_FORMATS) {
        VkFormatProperties	properties;
        vkGetPhysicalDeviceFormatProperties(physDevice, format, &properties);
        const bool hasLinearTiling = (bool)(properties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT);

        if (!hasLinearTiling) return false;
    }

    // Swap chain support for depth format
    for (const VkFormat format: SwapChain::DEPTH_FORMAT_CANDIDATES) {
        VkFormatProperties	properties;
        vkGetPhysicalDeviceFormatProperties(physDevice, format, &properties);

        VkFormatFeatureFlags features = 0;
        switch (SwapChain::DEPTH_TILING) {
            case VK_IMAGE_TILING_OPTIMAL:   features = properties.optimalTilingFeatures; break;
            case VK_IMAGE_TILING_LINEAR:    features = properties.linearTilingFeatures; break;
            default: break;
        }

        const bool hasRequiredSwapChainFeatures = (bool)(features & SwapChain::DEPTH_FEATURES);
        if (hasRequiredSwapChainFeatures) {
            SwapChain::setDepthFormat(format);
            break;
        }
    }

    return true;
}

/**
 * @brief Check if the physical device is suitable for the application,
 * given the requirements.
 * @param physDevice the physical device to check.
 */
static
bool _isDeviceSuitable(
    const QueueFamilyIndices& queueFamilyIndices,
    const VkPhysicalDevice physDevice,
    const VkSurfaceKHR surface
) {
    const bool  extensionsSupported = _hasExpectedExtensions(physDevice);
    bool        isSwapChainAdequate = false;

    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = _querySwapChainSupport(physDevice, surface);
        isSwapChainAdequate = !swapChainSupport.m_formats.empty() && !swapChainSupport.m_presentModes.empty();
    }

    return queueFamilyIndices.isComplete() &&
           extensionsSupported &&
           isSwapChainAdequate &&
           _hasExpectedFormatProperties(physDevice) &&
           _hasExpectedFeatures(physDevice);
}

void Device::_pickPhysicalDevice(const Core& core) {
    u32 physDeviceCount = 0;
    vkEnumeratePhysicalDevices(core.getInstance(), &physDeviceCount, nullptr);

    if (physDeviceCount == 0)
        throw std::runtime_error("Failed to find GPUs with Vulkan support");

    std::vector<VkPhysicalDevice> physDevices(physDeviceCount);
    vkEnumeratePhysicalDevices(core.getInstance(), &physDeviceCount, physDevices.data());

    for (const VkPhysicalDevice& physDevice : physDevices) {
        m_queueFamilyIndices = _findQueueFamilies(core.getSurface(), physDevice);
        if (_isDeviceSuitable(m_queueFamilyIndices, physDevice, core.getSurface())) {
            m_vkPhysicalDevice = physDevice;
            m_msaaCount = _getMaxUsableSampleCount(physDevice);
            return;
        }
    }

    throw std::runtime_error("Failed to find a suitable GPU");
}

/* ========================================================================== */

void Device::_createLogicalDevice() {
    const std::set<u32>         uniqueQueueFamilies = {
        m_queueFamilyIndices.m_graphicsFamily.value(),
        m_queueFamilyIndices.m_presentFamily.value(),
        m_queueFamilyIndices.m_computeFamily.value()
    };

    std::vector<VkDeviceQueueCreateInfo>    queueCreateInfos;

    for (u32 queueFamiliy: uniqueQueueFamilies) {
        const f32 queuePriority = 1.0f;
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamiliy;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.emplace_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures    deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo  deviceInfo{};
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.queueCreateInfoCount = static_cast<u32>(queueCreateInfos.size());
    deviceInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceInfo.pEnabledFeatures = &deviceFeatures;
    deviceInfo.enabledExtensionCount = static_cast<u32>(DEVICE_EXTENSIONS.size());
    deviceInfo.ppEnabledExtensionNames = DEVICE_EXTENSIONS.data();

    if (Core::ENABLE_VALIDATION_LAYERS) {
        deviceInfo.enabledLayerCount = (u32)(Core::VALIDATION_LAYERS.size());
        deviceInfo.ppEnabledLayerNames = Core::VALIDATION_LAYERS.data();
    }

    if (vkCreateDevice(m_vkPhysicalDevice, &deviceInfo, nullptr, &m_vkLogicalDevice) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create logical device!");
    }

    vkGetDeviceQueue(m_vkLogicalDevice, m_queueFamilyIndices.m_graphicsFamily.value(), 0, &m_queueFamilies.m_graphics);
    vkGetDeviceQueue(m_vkLogicalDevice, m_queueFamilyIndices.m_presentFamily.value(), 0, &m_queueFamilies.m_present);
    vkGetDeviceQueue(m_vkLogicalDevice, m_queueFamilyIndices.m_computeFamily.value(), 0, &m_queueFamilies.m_compute);
}

} // namespace vox::gfx