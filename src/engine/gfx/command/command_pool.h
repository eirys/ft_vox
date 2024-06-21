/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_pool.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 09:42:20 by etran             #+#    #+#             */
/*   Updated: 2024/06/12 11:35:27 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vulkan/vulkan.h>
#include "types.h"

namespace vox::gfx {

class Device;
class ICommandBuffer;
enum class CommandBufferType;

class CommandPool final {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    CommandPool() = default;
    ~CommandPool() = default;

    CommandPool(CommandPool&& other) = delete;
    CommandPool(const CommandPool& other) = delete;
    CommandPool& operator=(CommandPool&& other) = delete;
    CommandPool& operator=(const CommandPool& other) = delete;

    /* ====================================================================== */

    void             init(const Device& device);
    void             destroy(const Device& device);

    ICommandBuffer*  createCommandBuffer(
        const Device& device,
        const CommandBufferType type,
        const VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY) const;
    void             destroyBuffer(
        const Device& device,
        ICommandBuffer* buffer) const;

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    VkCommandPool   m_drawPool = VK_NULL_HANDLE;
    VkCommandPool   m_computePool = VK_NULL_HANDLE;

}; // class CommandPool

} // namespace vox::gfx
