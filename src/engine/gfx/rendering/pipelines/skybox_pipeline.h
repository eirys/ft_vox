/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skybox_pipeline.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 10:48:41 by etran             #+#    #+#             */
/*   Updated: 2024/06/14 19:37:00 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "pipeline.h"

namespace vox::gfx {

class SkyboxPipeline final: public Pipeline {
public:
    /* ====================================================================== */
    /*                                TYPEDEFS                                */
    /* ====================================================================== */

    using super = Pipeline;

    /* ====================================================================== */
    /*                                  ENUMS                                 */
    /* ====================================================================== */

    enum class ShaderStage: u32 {
        Vertex = 0,
        Fragment,

        First = Vertex,
        Last = Fragment
    };

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    SkyboxPipeline() = default;
    ~SkyboxPipeline() = default;

    SkyboxPipeline(SkyboxPipeline&& other) = delete;
    SkyboxPipeline(const SkyboxPipeline& other) = delete;
    SkyboxPipeline& operator=(SkyboxPipeline&& other) = delete;
    SkyboxPipeline& operator=(const SkyboxPipeline& other) = delete;

    /* ====================================================================== */

    void    init(
        const Device& device,
        const VkRenderPass& renderPass,
        const VkPipelineLayout& pipelineLayout) override;
    void    destroy(const Device& device) override;

    void    record(
        const PipelineLayout& pipeline,
        const ICommandBuffer* cmdBuffer) const override;

private:
    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static constexpr u32 SHADER_STAGE_COUNT = enumSize<ShaderStage>();

}; // class SkyboxPipeline

} // namespace vox::gfx