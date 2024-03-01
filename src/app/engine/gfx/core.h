/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 11:52:58 by etran             #+#    #+#             */
/*   Updated: 2024/03/01 12:08:57 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <vulkan/vulkan.h>

# include "types.h"
# include "debug_module.h"

namespace ui {
class Window;
}

namespace vox::gfx {

class Core final {
public:
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

    // VkPhysicalDevice            getPhysicalDevice() const noexcept;
    // const QueueFamilyIndices&   getQueueFamilyIndices() const noexcept;
    VkInstance                  getInstance() const noexcept;
    VkSurfaceKHR                getSurface() const noexcept;
    // u32                         getMsaaCount() const noexcept;

    /* ====================================================================== */

    // u32                         findMemoryType(u32 typeFilter, VkMemoryPropertyFlags properties) const;
    // SwapChainSupportDetails     querySwapChainSupport() const;

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    VkInstance          m_vkInstance;
    DebugModule         m_debugModule;

    VkSurfaceKHR        m_vkSurface = VK_NULL_HANDLE;
    // QueueFamilyIndices  m_queueFamilyIndices;

    // u32                 m_msaaCount = 1;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void                _createInstance();
    void                _createSurface(ui::Window& win);
    // void                _pickPhysicalDevice();

    // QueueFamilyIndices  _findQueueFamilies(VkPhysicalDevice physDevice) const;
    // bool                _isDeviceSuitable(const VkPhysicalDevice physDevice) const;

}; // class Core

} // namespace vox::gfx
