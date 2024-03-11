/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_render_pass.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:08:33 by etran             #+#    #+#             */
/*   Updated: 2024/03/07 16:06:25 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "render_pass.h"
#include "enum.h"

namespace vox::gfx {

/* ========================================================================== */
/*                                    ENUMS                                   */
/* ========================================================================== */

enum class SceneResource: u32 {
    ColorImage = 0,
    DepthImage,

    First = ColorImage,
    Last = DepthImage
};

enum class SceneAttachment: u32 {
    Color = 0,
    Depth,
    ColorResolve,

    First = Color,
    Last = ColorResolve
};

/* ========================================================================== */
/*                               HELPER OBJECTS                               */
/* ========================================================================== */

// Special case: SceneRenderPass needs swap chain image views to create the target.
struct SceneRenderPassInfo final: public RenderPassInfo {
    SceneRenderPassInfo(const std::vector<VkImageView>& swapViews): m_swapChainImageViews(swapViews) {}
    SceneRenderPassInfo() = delete;

    const std::vector<VkImageView>& m_swapChainImageViews;
};

class SceneRenderPass final: public RenderPass {
public:
    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static constexpr u32    RESOURCE_COUNT = enumSize<SceneResource>();
    static constexpr u32    ATTACHMENT_COUNT = enumSize<SceneAttachment>();

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    SceneRenderPass() = default;
    ~SceneRenderPass() = default;

    SceneRenderPass(SceneRenderPass&& other) = delete;
    SceneRenderPass(const SceneRenderPass& other) = delete;
    SceneRenderPass& operator=(SceneRenderPass&& other) = delete;
    SceneRenderPass& operator=(const SceneRenderPass& other) = delete;

    /* ====================================================================== */

    void    init(const Device& device, const RenderPassInfo* info) override;
    void    destroy(const Device& device) override;

    void    updateResources(const Device& device, const RenderPassInfo* info) override;
    void    begin(const ICommandBuffer* cmdBuffer, const RecordInfo& recordInfo) override;

private:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void    _createRenderPass(const Device& device, const RenderPassInfo* info);
    void    _createResources(const Device& device, const RenderPassInfo* info);
    void    _createTarget(const Device& device, const RenderPassInfo* info);

    void    _destroyTarget(const Device& device);
    void    _destroyResources(const Device& device);

}; // class SceneRenderPass

} // namespace vox::gfx
