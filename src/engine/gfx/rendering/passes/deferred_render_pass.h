/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deferred_render_pass.h                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:25:43 by etran             #+#    #+#             */
/*   Updated: 2024/06/25 12:29:53 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "vox_decl.h"
#include "render_pass.h"

namespace vox::gfx {

struct DeferredRenderPassInfo final: public RenderPassInfo {
    DeferredRenderPassInfo(
        const ImageBuffer& positionTexture,
        const ImageBuffer& normalTexture,
        const ImageBuffer& albedoTexture,
        const ImageBuffer& depthTexture
#if ENABLE_SSAO
        , const ImageBuffer& normalViewTexture
        , const ImageBuffer& posViewTexture
#endif
    ) :
        m_positionTexture(positionTexture),
        m_normalTexture(normalTexture),
        m_albedoTexture(albedoTexture),
        m_depth(depthTexture)
#if ENABLE_SSAO
        , m_normalViewTexture(normalViewTexture)
        , m_positionViewTexture(posViewTexture)
#endif
        {}

    DeferredRenderPassInfo() = delete;

    const ImageBuffer& m_positionTexture;
    const ImageBuffer& m_normalTexture;
    const ImageBuffer& m_albedoTexture;
    const ImageBuffer& m_depth;
#if ENABLE_SSAO
    const ImageBuffer& m_normalViewTexture;
    const ImageBuffer& m_positionViewTexture;
#endif
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
#if ENABLE_SSAO
        ColorViewNormal,
        ColorViewPosition,
#endif
        DepthImage,

        Count
    };

    enum class Attachment: u32 {
        ColorPosition,
        ColorNormal,
        ColorAlbedo,
#if ENABLE_SSAO
        ColorViewNormal,
        ColorViewPosition,
#endif
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