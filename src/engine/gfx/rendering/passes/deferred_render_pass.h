/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deferred_render_pass.h                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:25:43 by etran             #+#    #+#             */
/*   Updated: 2024/06/14 13:18:38 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "render_pass.h"

namespace vox::gfx {

struct DeferredRenderPassInfo final: public RenderPassInfo {
    DeferredRenderPassInfo(
        const ImageBuffer& positionTexture,
        const ImageBuffer& normalTexture,
        const ImageBuffer& albedoTexture):
        m_positionTexture(positionTexture),
        m_normalTexture(normalTexture),
        m_albedoTexture(albedoTexture) {};

    DeferredRenderPassInfo() = delete;

    const ImageBuffer& m_positionTexture;
    const ImageBuffer& m_normalTexture;
    const ImageBuffer& m_albedoTexture;
};

class DeferredRenderPass final: public RenderPass {
public:
    /* ====================================================================== */
    /*                                  ENUMS                                 */
    /* ====================================================================== */

    enum class Resource: u32 {
        ColorPosition,
        ColorNormal,
        ColorAlbedo,
        DepthImage,

        Count
    };

    enum class Attachment: u32 {
        ColorPosition,
        ColorNormal,
        ColorAlbedo,
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

    DeferredRenderPass() = default;
    ~DeferredRenderPass() = default;

    DeferredRenderPass(DeferredRenderPass&& other) = delete;
    DeferredRenderPass(const DeferredRenderPass& other) = delete;
    DeferredRenderPass& operator=(DeferredRenderPass&& other) = delete;
    DeferredRenderPass& operator=(const DeferredRenderPass& other) = delete;

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

}; // class DeferredRenderPass


} // namespace vox::gfx