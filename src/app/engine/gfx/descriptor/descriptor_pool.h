/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_pool.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:27:42 by etran             #+#    #+#             */
/*   Updated: 2024/02/28 22:25:57 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vulkan/vulkan.h>

namespace vox::gfx {

class Device;
class DescriptorTable;

class DescriptorPool final {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    DescriptorPool() = default;
    ~DescriptorPool() = default;

    DescriptorPool(DescriptorPool&& other) = delete;
    DescriptorPool(const DescriptorPool& other) = delete;
    DescriptorPool& operator=(DescriptorPool&& other) = delete;
    DescriptorPool& operator=(const DescriptorPool& other) = delete;

    /* ====================================================================== */

    void init(const Device& device, DescriptorTable& table);
    void destroy(const Device& device);

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    VkDescriptorPool    m_pool;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void _createPool(const Device& device, const DescriptorTable& table);
    void _allocatePool(const Device& device, DescriptorTable& table);

}; // class DescriptorPool

} // namespace vox::gfx