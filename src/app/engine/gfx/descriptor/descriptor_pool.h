/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_pool.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:27:42 by etran             #+#    #+#             */
/*   Updated: 2024/02/28 15:51:43 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vulkan/vulkan.h>

namespace vox::gfx {

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

    void init(const VkDevice& device);
    void destroy(const VkDevice& device);

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    VkDescriptorPool    m_pool;

}; // class DescriptorPool

} // namespace vox::gfx