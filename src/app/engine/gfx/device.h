/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   device.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 18:19:47 by etran             #+#    #+#             */
/*   Updated: 2024/02/27 17:29:07 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <vulkan/vulkan.h>

namespace vox::gfx {

class Core;
struct QueueFamilyIndices;

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

    const VkDevice& getDevice() const noexcept;
    VkDevice&       getDevice() noexcept;

    VkQueue         getGraphicsQueue() const noexcept;
    VkQueue         getPresentQueue() const noexcept;
    VkQueue         getComputeQueue() const noexcept;

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

    VkDevice        m_device;
    QueueFamilies   m_queueFamilies;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void    _createLogicalDevice(const Core& core);

}; // class Device

} // namespace vox::gfx