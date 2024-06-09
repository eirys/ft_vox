/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadow_render_pass.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 11:35:43 by etran             #+#    #+#             */
/*   Updated: 2024/06/03 17:18:48 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "render_pass.h"

namespace vox::gfx {

struct ShadowRenderPassInfo final: public RenderPassInfo {
    ShadowRenderPassInfo(const ImageBuffer& texture): m_texture(texture) {}
    ShadowRenderPassInfo() = delete;

    const ImageBuffer& m_texture;
};


class ShadowRenderPass final: public RenderPass {
public:
    /* ====================================================================== */
    /*                                  ENUMS                                 */
    /* ====================================================================== */

    enum class Resource: u32 {
        DepthImage,

        Count
    };

    enum class Attachment: u32 {
        Depth,

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

    ShadowRenderPass() = default;
    ~ShadowRenderPass() = default;

    ShadowRenderPass(ShadowRenderPass&& other) = delete;
    ShadowRenderPass(const ShadowRenderPass& other) = delete;
    ShadowRenderPass& operator=(ShadowRenderPass&& other) = delete;
    ShadowRenderPass& operator=(const ShadowRenderPass& other) = delete;

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
    void    _importResources(const Device& device, const RenderPassInfo* info);
    void    _createTarget(const Device& device, const RenderPassInfo* info);

    void    _destroyTarget(const Device& device);

}; // class ShadowRenderPass

} // namespace vox::gfx