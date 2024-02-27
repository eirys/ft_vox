/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icommand_buffer.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 10:28:53 by etran             #+#    #+#             */
/*   Updated: 2024/02/27 17:34:05 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vulkan/vulkan.h>

namespace vox::gfx {

class Device;

class ICommandBuffer {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    virtual ~ICommandBuffer() = default;

    /* ====================================================================== */

    virtual void            init(const Device& device, const VkCommandBufferLevel level) = 0;
    virtual void            destroy(const Device& device) = 0;

    virtual void            startRecording(VkCommandBufferUsageFlags flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT) = 0;
    virtual void            stopRecording(const Device& device, bool await = false) = 0;
    virtual void            reset() = 0;

    virtual VkCommandBuffer getBuffer() const noexcept = 0;

protected:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    ICommandBuffer() = default;

    ICommandBuffer(ICommandBuffer&& other) = delete;
    ICommandBuffer(const ICommandBuffer& other) = delete;
    ICommandBuffer& operator=(ICommandBuffer&& other) = delete;
    ICommandBuffer& operator=(const ICommandBuffer& other) = delete;

}; // class ICommandBuffer

} // namespace vox::gfx