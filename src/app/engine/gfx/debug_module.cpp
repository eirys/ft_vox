/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_module.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:30:51 by etran             #+#    #+#             */
/*   Updated: 2024/03/01 11:46:56 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug_module.h"
#include <stdexcept>
#include <iostream>

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

void DebugModule::init(VkInstance instance) {
    if (!ENABLE_VALIDATION_LAYERS) return;

    VkDebugUtilsMessengerCreateInfoEXT debugInfo = getMessengerCreateInfo();
    if (_createDebugUtilsMessengerExt(instance, &debugInfo, nullptr, &m_debugMessenger) != VK_SUCCESS)
        throw std::runtime_error("failed to set up debug messenger");
}

void DebugModule::destroy(VkInstance instance) {
    if (!ENABLE_VALIDATION_LAYERS) return;

    _destroyDebugUtilsMessengerExt(instance, m_debugMessenger, nullptr);
}

VkDebugUtilsMessengerCreateInfoEXT DebugModule::getMessengerCreateInfo() {
    VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    #ifdef __VERBOSE
    createInfo.messageSeverity |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
    createInfo.messageSeverity |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
    #endif

    createInfo.messageType =
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

    createInfo.pfnUserCallback = _debugCallback;
    createInfo.pUserData = nullptr;
    return createInfo;
}

} // namespace vox::gfx
