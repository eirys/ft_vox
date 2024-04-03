/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_tex_pipeline.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 22:06:00 by etran             #+#    #+#             */
/*   Updated: 2024/04/03 22:16:33 by etran            ###   ########.fr       */
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

    ScenePipeline() = default;
    ~ScenePipeline() = default;

    ScenePipeline(ScenePipeline&& other) = delete;
    ScenePipeline(const ScenePipeline& other) = delete;
    ScenePipeline& operator=(ScenePipeline&& other) = delete;
    ScenePipeline& operator=(const ScenePipeline& other) = delete;

    /* ====================================================================== */

    void    init(
        const Device& device,
        const RenderPass* renderPass,
        const VkPipelineLayout& pipelineLayout) override;
    void    destroy(const Device& device) override;

    void    record(
        const VkPipelineLayout layout,
        const DescriptorTable& descriptorTable,
        const ICommandBuffer* cmdBuffer) override;

private:
    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static constexpr u32 SHADER_STAGE_COUNT = enumSize<ShaderStage>();

}; // class DebugTexPipeline

}  // namespace vox::gfx