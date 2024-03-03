/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_pass.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 19:56:22 by etran             #+#    #+#             */
/*   Updated: 2024/03/02 21:54:50 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vulkan/vulkan.h>
#include "types.h"

namespace vox::gfx {

class Device;
class ImageBuffer;
class RenderPassUpdateInfo;

class RenderPass {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    virtual ~RenderPass() = default;

    /* ====================================================================== */

    virtual void    init(const Device& device) = 0;
    virtual void    destroy(const Device& device) = 0;

    /* ====================================================================== */

    virtual void    updateResources(
        const Device& device,
        const RenderPassUpdateInfo& updateInfo) = 0;

    /* ====================================================================== */

    VkRenderPass    getRenderPass() const noexcept;
    u32             getWidth() const noexcept;
    u32             getHeight() const noexcept;

protected:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    VkRenderPass    m_vkRenderPass = VK_NULL_HANDLE;
    u32             m_width = 0;
    u32             m_height = 0;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    RenderPass() = default;

    RenderPass(RenderPass&& other) = delete;
    RenderPass(const RenderPass& other) = delete;
    RenderPass& operator=(RenderPass&& other) = delete;
    RenderPass& operator=(const RenderPass& other) = delete;

}; // class RenderPass

} // namespace vox::gfx
