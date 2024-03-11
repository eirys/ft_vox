/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 11:52:58 by etran             #+#    #+#             */
/*   Updated: 2024/03/11 11:26:35 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <vulkan/vulkan.h>
# include <array>

# include "types.h"

namespace ui {
class Window;
}

namespace vox::gfx {

class Core final {
public:
    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static constexpr std::array<const char*, 1> VALIDATION_LAYERS = { "VK_LAYER_KHRONOS_validation" };

#ifndef __RELEASE
    static constexpr bool   ENABLE_VALIDATION_LAYERS = true;
#else
    static constexpr bool   ENABLE_VALIDATION_LAYERS = false;
#endif

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

    VkInstance                  getInstance() const noexcept;
    VkSurfaceKHR                getSurface() const noexcept;

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    VkInstance                  m_vkInstance = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT    m_vkDebugMessenger = VK_NULL_HANDLE;

    VkSurfaceKHR                m_vkSurface = VK_NULL_HANDLE;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void                _createInstance();
    void                _setupDebugMessenger();
    void                _createSurface(ui::Window& win);

}; // class Core

} // namespace vox::gfx
