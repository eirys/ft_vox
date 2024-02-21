/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   device.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 18:19:47 by etran             #+#    #+#             */
/*   Updated: 2024/02/21 23:47:25 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <vulkan/vulkan.h>

namespace vox {

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

    void    init();
    void    destroy();

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

    void    _createLogicalDevice(QueueFamilyIndices& indices);
    void    _createQueueFamilies(QueueFamilyIndices& indices);

}; // class Device

} // namespace vox