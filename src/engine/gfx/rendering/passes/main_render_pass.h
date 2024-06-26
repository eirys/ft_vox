/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_render_pass.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:08:33 by etran             #+#    #+#             */
/*   Updated: 2024/06/03 16:10:08 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "render_pass.h"
#include "enum.h"

namespace vox::gfx {

/* ========================================================================== */
/*                               HELPER OBJECTS                               */
/* ========================================================================== */

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
        ColorImage,
        DepthImage,

        Count
    };

    enum class Attachment: u32 {
        Color,
        Depth,
        ColorResolve,

        Count
    };

    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static constexpr u32    RESOURCE_COUNT = (u32)Resource::Count;
    static constexpr u32    ATTACHMENT_COUNT = (u32)Attachment::Count;

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
    void    begin(const ICommandBuffer* cmdBuffer, const RecordInfo& recordInfo) const override;

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
