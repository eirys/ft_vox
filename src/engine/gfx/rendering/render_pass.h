/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_pass.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 19:56:22 by etran             #+#    #+#             */
/*   Updated: 2024/03/07 11:45:29 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "types.h"
#include "image_buffer.h"

namespace vox::gfx {

class Device;

/* ========================================================================== */
/*                               HELPER OBJECTS                               */
/* ========================================================================== */

struct RenderPassInfo {
    virtual ~RenderPassInfo() = default;

    // Resources
    std::vector<VkFormat>               m_formats;
    std::vector<VkSampleCountFlagBits>  m_samples;

    // Render pass
    u32 m_renderPassWidth;
    u32 m_renderPassHeight;

    // Framebuffer
    u32 m_targetCount;
    u32 m_targetWidth;
    u32 m_targetHeight;
};

/**
 * @brief Wrapper class for a Vulkan render pass.
*/
class RenderPass {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    virtual ~RenderPass() = default;

    /* ====================================================================== */

    virtual void    init(const Device& device, const RenderPassInfo& info) = 0;
    virtual void    destroy(const Device& device) = 0;

    virtual void    updateResources(const Device& device, const RenderPassInfo& info) = 0;

    /* ====================================================================== */

    VkRenderPass    getRenderPass() const noexcept;
    u32             getWidth() const noexcept;
    u32             getHeight() const noexcept;

    const std::vector<VkFramebuffer>&   getTargets() const noexcept;
    const std::vector<ImageBuffer>&     getResources() const noexcept;

protected:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    VkRenderPass                m_vkRenderPass = VK_NULL_HANDLE;
    std::vector<VkFramebuffer>  m_targets;
    std::vector<ImageBuffer>    m_resources;

    u32                         m_width = 0;
    u32                         m_height = 0;

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
