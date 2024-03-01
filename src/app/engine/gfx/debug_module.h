/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_module.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:30:40 by etran             #+#    #+#             */
/*   Updated: 2024/03/01 12:06:57 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vulkan/vulkan.h>
#include <array>

namespace vox::gfx {

class DebugModule final {
public:
    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static constexpr std::array<const char*, 1> VALIDATION_LAYERS = {
        "VK_LAYER_KHRONOS_validation"
    };

#ifdef __RELEASE
    static constexpr bool   ENABLE_VALIDATION_LAYERS = false;
#else
    static constexpr bool   ENABLE_VALIDATION_LAYERS = true;
#endif

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    DebugModule() = default;
    ~DebugModule() = default;

    DebugModule(DebugModule&& other) = delete;
    DebugModule(const DebugModule& other) = delete;
    DebugModule& operator=(DebugModule&& other) = delete;
    DebugModule& operator=(const DebugModule& other) = delete;

    /* ====================================================================== */

    void    init(VkInstance instance);
    void    destroy(VkInstance instance);

    static
    VkDebugUtilsMessengerCreateInfoEXT    getMessengerCreateInfo();

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    VkDebugUtilsMessengerEXT    m_debugMessenger;

}; // class DebugModule

} // namespace vox::gfx