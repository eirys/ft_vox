/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 11:53:00 by etran             #+#    #+#             */
/*   Updated: 2024/03/02 00:23:08 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.h"
#include "window.h"

#include <cstring>
#include <stdexcept>
#include <vector>

#include "debug.h"

namespace vox::gfx {

/* ========================================================================== */
/*                                    OTHER                                   */
/* ========================================================================== */

static
VkResult _createDebugUtilsMessengerExt(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger
) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr)
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    else
        return VK_ERROR_EXTENSION_NOT_PRESENT;
}

static
void _destroyDebugUtilsMessengerExt(
    const VkInstance instance,
    VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks* pAllocator
) {
    if (debugMessenger == VK_NULL_HANDLE) return;

    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance,"vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr)
        func(instance, debugMessenger, pAllocator);
}

static VKAPI_ATTR VKAPI_CALL
VkBool32 _debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData
) {
    (void)messageType;
    (void)pUserData;

    static constexpr std::array<const char*, 4> severityIndicator = {
        "VERBOSE",
        "INFO",
        "WARNING",
        "ERROR" };

    const int index = (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) ? 0 :
                      (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) ? 1 :
                      (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) ? 2 : 3;

    std::cerr   << "[* VK_VL | " << severityIndicator[index] << " *] "
                << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void Core::init(ui::Window& win) {
    _createInstance();
    _setupDebugMessenger();
    _createSurface(win);
}

void Core::destroy() {
    vkDestroySurfaceKHR(m_vkInstance, m_vkSurface, nullptr);
    _destroyDebugUtilsMessengerExt(m_vkInstance, m_vkDebugMessenger, nullptr);
    vkDestroyInstance(m_vkInstance, nullptr);
}

/* ========================================================================== */

VkInstance Core::getInstance() const noexcept {
    return m_vkInstance;
}

VkSurfaceKHR Core::getSurface() const noexcept {
    return m_vkSurface;
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

static
constexpr VkDebugUtilsMessengerCreateInfoEXT _getMessengerCreateInfo() {
    constexpr VkDebugUtilsMessengerCreateInfoEXT createInfo {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .pNext = nullptr,
        .flags = 0,
        .messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            #ifdef __VERBOSE
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
            #endif
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        .pfnUserCallback = _debugCallback,
        .pUserData = nullptr,
    };

    return createInfo;
}

static
std::vector<const char*> _getRequiredExtensions() {
    u32             glfwExtensionCount = 0;
    const char**    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (Core::ENABLE_VALIDATION_LAYERS)
        extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    return extensions;
}

static
bool _hasValidationLayerSupport() {
    u32 layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : Core::VALIDATION_LAYERS) {
        for (const VkLayerProperties& layerProperties: availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0)
                return true;
        }
    }
    return false;
}

void Core::_createInstance() {
    if (ENABLE_VALIDATION_LAYERS && !_hasValidationLayerSupport())
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

    const std::vector<const char*> extensions = _getRequiredExtensions();
    instanceInfo.enabledExtensionCount = (u32)(extensions.size());
    instanceInfo.ppEnabledExtensionNames = extensions.data();

    if (ENABLE_VALIDATION_LAYERS) {
        constexpr VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = _getMessengerCreateInfo();
        instanceInfo.enabledLayerCount = (u32)VALIDATION_LAYERS.size();
        instanceInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
        instanceInfo.pNext = &debugCreateInfo;
    }

    if (vkCreateInstance(&instanceInfo, nullptr, &m_vkInstance) != VK_SUCCESS)
        throw std::runtime_error("Failed to create instance");
}

/* ========================================================================== */

void Core::_setupDebugMessenger() {
    if (!ENABLE_VALIDATION_LAYERS) return;

    constexpr VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = _getMessengerCreateInfo();
    if (_createDebugUtilsMessengerExt(m_vkInstance, &debugCreateInfo, nullptr, &m_vkDebugMessenger) != VK_SUCCESS)
        throw std::runtime_error("failed to set up debug messenger");
}

/* ========================================================================== */

void Core::_createSurface(ui::Window& win) {
    if (glfwCreateWindowSurface(m_vkInstance, win.getWindow(), nullptr, &m_vkSurface) != VK_SUCCESS)
        throw std::runtime_error("Failed to create window surface");
}

} // namespace vox::gfx
