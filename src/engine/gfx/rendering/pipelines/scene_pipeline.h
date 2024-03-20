/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_pipeline.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 23:49:38 by etran             #+#    #+#             */
/*   Updated: 2024/03/19 11:33:25 by etran            ###   ########.fr       */
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

    ScenePipeline();
    ~ScenePipeline();

    ScenePipeline(ScenePipeline&& other) = delete;
    ScenePipeline(const ScenePipeline& other) = delete;
    ScenePipeline& operator=(ScenePipeline&& other) = delete;
    ScenePipeline& operator=(const ScenePipeline& other) = delete;

    /* ====================================================================== */

    void    init(
        const Device& device,
        const RenderPassInfo* info,
        const VkPipelineLayout& pipelineLayout) override;
    void    destroy(const Device& device) override;

    void    record(
        const VkPipelineLayout layout,
        const DescriptorTable& descriptorTable,
        const ICommandBuffer* cmdBuffer,
        const RecordInfo& drawInfo) override;

    RenderPass* getRenderPass() const noexcept override;

private:
    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static constexpr u32 SHADER_STAGE_COUNT = enumSize<ShaderStage>();

    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    SceneRenderPass* m_renderPass = nullptr;

}; // class ScenePipeline

} // namespace vox::gfx
