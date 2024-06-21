/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ssao_blur_render_pass.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 21:06:14 by etran             #+#    #+#             */
/*   Updated: 2024/06/16 21:07:22 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "render_pass.h"

namespace vox::gfx {

struct SSAOBlurRenderPassInfo final: public RenderPassInfo {
    SSAOBlurRenderPassInfo(const ImageBuffer& texture):
        m_texture(texture) {};

    SSAOBlurRenderPassInfo() = delete;

    const ImageBuffer& m_texture;
};

class SSAOBlurRenderPass final: public RenderPass {
public:
    /* ====================================================================== */
    /*                                  ENUMS                                 */
    /* ====================================================================== */

    enum class Resource: u32 {
        Color,

        Count
    };

    enum class Attachment: u32 {
        Color,

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

    SSAOBlurRenderPass() = default;
    ~SSAOBlurRenderPass() = default;

    SSAOBlurRenderPass(SSAOBlurRenderPass&& other) = delete;
    SSAOBlurRenderPass(const SSAOBlurRenderPass& other) = delete;
    SSAOBlurRenderPass& operator=(SSAOBlurRenderPass&& other) = delete;
    SSAOBlurRenderPass& operator=(const SSAOBlurRenderPass& other) = delete;

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
    void    _createTargets(const Device& device, const RenderPassInfo* info);

    void    _destroyTargets(const Device& device);

}; // class SSAOBlurRenderPass

} // namespace vox::gfx