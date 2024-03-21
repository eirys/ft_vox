/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_render_pass.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:08:33 by etran             #+#    #+#             */
/*   Updated: 2024/03/21 01:35:10 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "render_pass.h"
#include "enum.h"

namespace vox::gfx {

/* ========================================================================== */
/*                               HELPER OBJECTS                               */
/* ========================================================================== */

// Special case: MainRenderPass needs swap chain image views to create the target.
struct MainRenderPassInfo final: public RenderPassInfo {
    MainRenderPassInfo(const std::vector<VkImageView>& swapViews): m_swapChainImageViews(swapViews) {}
    MainRenderPassInfo() = delete;

    const std::vector<VkImageView>& m_swapChainImageViews;
};

class MainRenderPass final: public RenderPass {
public:
    /* ====================================================================== */
    /*                                  ENUMS                                 */
    /* ====================================================================== */

    enum class Resource: u32 {
        ColorImage = 0,
        DepthImage,

        First = ColorImage,
        Last = DepthImage
    };

    enum class Attachment: u32 {
        Color = 0,
        Depth,
        ColorResolve,

        First = Color,
        Last = ColorResolve
    };

    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static constexpr u32    RESOURCE_COUNT = enumSize<Resource>();
    static constexpr u32    ATTACHMENT_COUNT = enumSize<Attachment>();

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    MainRenderPass() = default;
    ~MainRenderPass() = default;

    MainRenderPass(MainRenderPass&& other) = delete;
    MainRenderPass(const MainRenderPass& other) = delete;
    MainRenderPass& operator=(MainRenderPass&& other) = delete;
    MainRenderPass& operator=(const MainRenderPass& other) = delete;

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

}; // class MainRenderPass

} // namespace vox::gfx
