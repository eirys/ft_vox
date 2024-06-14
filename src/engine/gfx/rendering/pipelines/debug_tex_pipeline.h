/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_tex_pipeline.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 22:06:00 by etran             #+#    #+#             */
/*   Updated: 2024/06/14 18:48:29 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "pipeline.h"

namespace vox::gfx {

/**
 * @brief Debug texture pipeline.
 * Displays the wanted texture on the screen.
 */
class DebugTexPipeline final: public Pipeline {
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

    DebugTexPipeline() = default;
    ~DebugTexPipeline() = default;

    DebugTexPipeline(DebugTexPipeline&& other) = delete;
    DebugTexPipeline(const DebugTexPipeline& other) = delete;
    DebugTexPipeline& operator=(DebugTexPipeline&& other) = delete;
    DebugTexPipeline& operator=(const DebugTexPipeline& other) = delete;

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

}; // class DebugTexPipeline

}  // namespace vox::gfx