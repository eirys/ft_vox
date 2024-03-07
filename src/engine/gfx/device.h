/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   device.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 18:19:47 by etran             #+#    #+#             */
/*   Updated: 2024/03/07 13:21:26 by etran            ###   ########.fr       */
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
    std::optional<u32>  m_graphicsFamily;
    std::optional<u32>  m_presentFamily;
    std::optional<u32>  m_computeFamily;

    bool isComplete() const {
        return  m_graphicsFamily.has_value() &&
                m_computeFamily.has_value() &&
                m_presentFamily.has_value();
    }
};

struct SwapChainSupportDetails final {
    VkSurfaceCapabilitiesKHR        m_capabilities;
    std::vector<VkSurfaceFormatKHR> m_formats;
    std::vector<VkPresentModeKHR>   m_presentModes;
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
    void    idle() const;

    /* ====================================================================== */

    u32                         findMemoryType(const u32 typeFilter, const VkMemoryPropertyFlags properties) const;
    SwapChainSupportDetails     querySwapChainSupport(const Core& core) const;

    /* ====================================================================== */

    VkDevice                    getDevice() const noexcept;
    VkQueue                     getGraphicsQueue() const noexcept;
    VkQueue                     getPresentQueue() const noexcept;
    VkQueue                     getComputeQueue() const noexcept;
    VkSampleCountFlagBits       getMsaaCount() const noexcept;

    const QueueFamilyIndices&   getQueueFamilyIndices() const noexcept;

private:
    /* ====================================================================== */
    /*                              HELPER CLASS                              */
    /* ====================================================================== */

    struct QueueFamilies final {
        VkQueue m_graphics = VK_NULL_HANDLE;
        VkQueue m_present = VK_NULL_HANDLE;
        VkQueue m_compute = VK_NULL_HANDLE;
    };

    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    VkPhysicalDevice        m_vkPhysicalDevice = VK_NULL_HANDLE;
    VkDevice                m_vkLogicalDevice = VK_NULL_HANDLE;

    QueueFamilyIndices      m_queueFamilyIndices;
    QueueFamilies           m_queueFamilies;
    VkSampleCountFlagBits   m_msaaCount = VK_SAMPLE_COUNT_1_BIT;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void                _pickPhysicalDevice(const Core& core);
    void                _createLogicalDevice();

}; // class Device

} // namespace vox::gfx