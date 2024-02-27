/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 11:52:58 by etran             #+#    #+#             */
/*   Updated: 2024/02/27 18:23:54 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <vulkan/vulkan.h>
# include <optional>
# include <array>
# include <vector>

# include "types.h"
# include "debug_module.h"

namespace ui {
class Window;
}

namespace vox::gfx {

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

class Core final {
public:
    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static constexpr
    std::array<const char*, 1>  DEVICE_EXTENSIONS = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    Core() = default;
    ~Core() = default;

    Core(Core&& other) = delete;
    Core(const Core& other) = delete;
    Core& operator=(Core&& other) = delete;
    Core& operator=(const Core& other) = delete;

    /* ====================================================================== */

    void        init(ui::Window& win);
    void        destroy();

    /* ====================================================================== */

    VkPhysicalDevice            getPhysicalDevice() const noexcept;
    const QueueFamilyIndices&   getQueueFamilyIndices() const noexcept;
    u32                         getMsaaCount() const noexcept;

    /* ====================================================================== */

    u32                         findMemoryType(u32 typeFilter, VkMemoryPropertyFlags properties) const;
    SwapChainSupportDetails     querySwapChainSupport() const;

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    VkInstance          m_vkInstance;
    DebugModule         m_debugModule;

    VkSurfaceKHR        m_vkSurface = VK_NULL_HANDLE;
    VkPhysicalDevice    m_vkPhysicalDevice = VK_NULL_HANDLE;
    QueueFamilyIndices  m_queueFamilyIndices;

    u32                 m_msaaCount = 1;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void                _createInstance();
    void                _createSurface(ui::Window& win);
    void                _pickPhysicalDevice();

    QueueFamilyIndices  _findQueueFamilies(VkPhysicalDevice physDevice) const;
    bool                _isDeviceSuitable(VkPhysicalDevice physDevice) const;

}; // class Core

} // namespace vox::gfx
