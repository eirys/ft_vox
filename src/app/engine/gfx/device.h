/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   device.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 18:19:47 by etran             #+#    #+#             */
/*   Updated: 2024/03/01 00:02:12 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <vulkan/vulkan.h>

# include <optional>
# include <vector>

# include "types.h"

namespace vox::gfx {

class Core;

/* ========================================================================== */
/*                               HELPER OBJECTS                               */
/* ========================================================================== */

struct QueueFamilyIndices final {
    std::optional<u32>  graphicsFamily;
    std::optional<u32>  presentFamily;
    std::optional<u32>  computeFamily;

    bool isComplete() const {
        return  graphicsFamily.has_value() &&
                computeFamily.has_value() &&
                presentFamily.has_value();
    }
};

struct SwapChainSupportDetails final {
    VkSurfaceCapabilitiesKHR        capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR>   presentModes;
};

class Device final {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    Device() = default;
    ~Device() = default;

    Device(Device&& other) = delete;
    Device(const Device& other) = delete;
    Device& operator=(Device&& other) = delete;
    Device& operator=(const Device& other) = delete;

    /* ====================================================================== */

    void    init(const Core& core);
    void    destroy();

    /* ====================================================================== */

    u32                         findMemoryType(const u32 typeFilter, const VkMemoryPropertyFlags properties) const;
    SwapChainSupportDetails     querySwapChainSupport(const Core& core) const;

    /* ====================================================================== */

    const VkDevice& getDevice() const noexcept;
    VkDevice&       getDevice() noexcept;

    VkQueue         getGraphicsQueue() const noexcept;
    VkQueue         getPresentQueue() const noexcept;
    VkQueue         getComputeQueue() const noexcept;
    u32             getMsaaCount() const noexcept;

    const QueueFamilyIndices&   getQueueFamilyIndices() const noexcept;

private:
    /* ====================================================================== */
    /*                              HELPER CLASS                              */
    /* ====================================================================== */

    struct QueueFamilies final {
        VkQueue graphics;
        VkQueue present;
        VkQueue compute;
    };

    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    VkPhysicalDevice    m_vkPhysicalDevice = VK_NULL_HANDLE;
    VkDevice            m_vkLogicalDevice = VK_NULL_HANDLE;

    QueueFamilies       m_queueFamilies;
    u32                 m_msaaCount = 1;
    QueueFamilyIndices  m_queueFamilyIndices;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void                _pickPhysicalDevice(const Core& core);
    void                _createLogicalDevice();

    // QueueFamilyIndices  _findQueueFamilies(
    //     const VkSurfaceKHR surface,
    //     const VkPhysicalDevice physDevice) const;
    bool                _isDeviceSuitable(
        const VkPhysicalDevice physDevice,
        const VkSurfaceKHR surface) const;

}; // class Device

} // namespace vox::gfx