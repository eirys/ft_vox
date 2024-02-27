/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_module.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:30:51 by etran             #+#    #+#             */
/*   Updated: 2024/02/27 17:29:26 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug_module.h"
#include <stdexcept>
#include <iostream>

namespace vox::gfx {

/* ========================================================================== */
/*                               STATIC MEMBERS                               */
/* ========================================================================== */

const std::array<const char*, 1>	DebugModule::sm_validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

/* ========================================================================== */
/*                                    OTHER                                   */
/* ========================================================================== */

static
VkResult    _createDebugUtilsMessengerExt(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger
) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

static VKAPI_ATTR VKAPI_CALL
VkBool32 _debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData
) {
    (void)messageSeverity;
    (void)messageType;
    (void)pUserData;
    std::cerr << "[validation layer] " << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}


/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void DebugModule::init(VkInstance instance) {
    if (!ENABLE_VALIDATION_LAYERS) {
        return;
    }

    VkDebugUtilsMessengerCreateInfoEXT debugInfo = getMessengerCreateInfo();
    if (_createDebugUtilsMessengerExt(instance, &debugInfo, nullptr, &m_debugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug messenger");
    }
}

void DebugModule::destroy(VkInstance instance) {
    if (!ENABLE_VALIDATION_LAYERS) {
        return;
    }

    if (ENABLE_VALIDATION_LAYERS) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(instance, m_debugMessenger, nullptr);
        }
    }
}

VkDebugUtilsMessengerCreateInfoEXT DebugModule::getMessengerCreateInfo() {
    VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    #ifdef __VERBOSE
    createInfo.messageSeverity |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
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
