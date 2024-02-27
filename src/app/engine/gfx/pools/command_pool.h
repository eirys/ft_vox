/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_pool.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 09:42:20 by etran             #+#    #+#             */
/*   Updated: 2024/02/27 17:33:20 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vulkan/vulkan.h>
#include "types.h"

namespace vox::gfx {

class Core;
class Device;
class ICommandBuffer;

enum class CommandBufferType: u8 {
    DRAW,
    COMPUTE,
};

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

    static void             init(const Core& core, const Device& device);
    static void             destroy(const Device& device);

    static ICommandBuffer*  createCommandBuffer(
        Device& device,
        CommandBufferType type,
        VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
    static void             destroyBuffer(
        Device& device,
        ICommandBuffer* buffer);

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    static VkCommandPool m_drawPool;
    static VkCommandPool m_computePool;

}; // class CommandPool

} // namespace vox::gfx
