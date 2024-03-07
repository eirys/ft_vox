/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_pipeline.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 23:49:38 by etran             #+#    #+#             */
/*   Updated: 2024/03/07 10:24:43 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "pipeline.h"

namespace vox::gfx {

class SceneRenderPass;

class ScenePipeline final: public Pipeline {
public:
    /* ====================================================================== */
    /*                                TYPEDEFS                                */
    /* ====================================================================== */

    using super = Pipeline;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void    init(const Device& device) override;
    void    destroy(const Device& device) override;

    void    record(
        const VkPipelineLayout layout,
        const ICommandBuffer* cmdBuffer,
        const IPipelineRenderInfo* drawInfo) override;

    /* ====================================================================== */

    RenderPass* getRenderPass() const noexcept override;

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    SceneRenderPass* m_renderPass = nullptr;

}; // class ScenePipeline

} // namespace vox::gfx
