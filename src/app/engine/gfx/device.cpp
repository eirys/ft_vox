/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   device.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 23:37:02 by etran             #+#    #+#             */
/*   Updated: 2024/02/27 17:29:15 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "device.h"
#include "core.h"
#include "types.h"

#include <set>
#include <stdexcept>

namespace vox::gfx {
/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void Device::init(const Core& core) {
    _createLogicalDevice(core);
}

void Device::destroy() {
    vkDestroyDevice(m_device, nullptr);
}

/* ========================================================================== */

const VkDevice& Device::getDevice() const noexcept {
    return m_device;
}

VkDevice& Device::getDevice() noexcept {
    return m_device;
}

VkQueue Device::getGraphicsQueue() const noexcept {
    return m_queueFamilies.graphics;
}

VkQueue Device::getPresentQueue() const noexcept {
    return m_queueFamilies.present;
}

VkQueue Device::getComputeQueue() const noexcept {
    return m_queueFamilies.compute;
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

void Device::_createLogicalDevice(const Core& core) {
    const QueueFamilyIndices&   indices = core.getQueueFamilyIndices();
    const std::set<u32>         uniqueQueueFamilies = {
        indices.graphicsFamily.value(),
        indices.presentFamily.value(),
        indices.computeFamily.value()
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
    deviceInfo.enabledExtensionCount = static_cast<u32>(Core::DEVICE_EXTENSIONS.size());
    deviceInfo.ppEnabledExtensionNames = Core::DEVICE_EXTENSIONS.data();

    if (DebugModule::ENABLE_VALIDATION_LAYERS) {
        deviceInfo.enabledLayerCount = static_cast<u32>(DebugModule::sm_validationLayers.size());
        deviceInfo.ppEnabledLayerNames = DebugModule::sm_validationLayers.data();
    }

    if (vkCreateDevice(core.getPhysicalDevice(), &deviceInfo, nullptr, &m_device) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create logical device!");
    }

    vkGetDeviceQueue(m_device, indices.graphicsFamily.value(), 0, &m_queueFamilies.graphics);
    vkGetDeviceQueue(m_device, indices.presentFamily.value(), 0, &m_queueFamilies.present);
    vkGetDeviceQueue(m_device, indices.computeFamily.value(), 0, &m_queueFamilies.compute);
}


} // namespace vox::gfx