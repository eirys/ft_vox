/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 11:53:00 by etran             #+#    #+#             */
/*   Updated: 2024/03/11 14:35:44 by etran            ###   ########.fr       */
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

static
VKAPI_ATTR VkBool32 VKAPI_CALL _debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData
) {
    (void)pUserData;

#ifndef __VERBOSE
    if (messageType != VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
        return VK_FALSE;
#endif

    static constexpr std::array<const char*, 4> severityIndicator = {
        "VERBOSE",
        "INFO",
        "WARNING",
        "ERROR" };

#ifdef __VERBOSE
    static constexpr std::array<const char*, 4> typeIndicator = {
        "General",
        "Validation",
        "Performance" };
#endif

    const u32 severityIndex =
        (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) ? 0 :
        (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) ? 1 :
        (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) ? 2 : 3;

#ifdef __VERBOSE
    const u32 typeIndex =
        (messageType == VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT) ? 0 :
        (messageType == VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) ? 1 : 2;
#endif

    std::cerr   << "[*VK_VL*]["
#ifdef __VERBOSE
                << typeIndicator[typeIndex] << "::"
#endif
                << severityIndicator[severityIndex] <<  "] "
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

    LDEBUG("Core initialized");
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
    VkDebugUtilsMessengerCreateInfoEXT createInfo {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
#ifdef __VERBOSE
    createInfo.messageSeverity |=
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
#endif
    createInfo.messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = _debugCallback;
    createInfo.pUserData = nullptr;

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
        bool layerFound = false;
        for (const VkLayerProperties& layerProperties: availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }
        if (!layerFound) return false;
    }
    return true;
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

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    if (ENABLE_VALIDATION_LAYERS) {
        debugCreateInfo = _getMessengerCreateInfo();
        instanceInfo.enabledLayerCount = (u32)VALIDATION_LAYERS.size();
        instanceInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
        instanceInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    } else {
        instanceInfo.enabledLayerCount = 0;
        instanceInfo.pNext = nullptr;
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
