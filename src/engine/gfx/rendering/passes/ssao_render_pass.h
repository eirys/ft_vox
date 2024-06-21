/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ssao_render_pass.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 18:00:41 by etran             #+#    #+#             */
/*   Updated: 2024/06/15 20:11:47 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "render_pass.h"

namespace vox::gfx {

struct SSAORenderPassInfo final: public RenderPassInfo {
    SSAORenderPassInfo(const ImageBuffer& texture):
        m_texture(texture) {};

    SSAORenderPassInfo() = delete;

    const ImageBuffer& m_texture;
};

class SSAORenderPass final: public RenderPass {
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

    SSAORenderPass() = default;
    ~SSAORenderPass() = default;

    SSAORenderPass(SSAORenderPass&& other) = delete;
    SSAORenderPass(const SSAORenderPass& other) = delete;
    SSAORenderPass& operator=(SSAORenderPass&& other) = delete;
    SSAORenderPass& operator=(const SSAORenderPass& other) = delete;

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

}; // class SSAORenderPass

} // namespace vox::gfx