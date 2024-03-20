/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skybox_pipeline.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 10:48:41 by etran             #+#    #+#             */
/*   Updated: 2024/03/19 11:33:19 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "pipeline.h"

namespace vox::gfx {

class RenderPass;

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

    SkyboxPipeline(RenderPass* renderPass);

    ~SkyboxPipeline() = default;

    SkyboxPipeline() = delete;
    SkyboxPipeline(SkyboxPipeline&& other) = delete;
    SkyboxPipeline(const SkyboxPipeline& other) = delete;
    SkyboxPipeline& operator=(SkyboxPipeline&& other) = delete;
    SkyboxPipeline& operator=(const SkyboxPipeline& other) = delete;

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

    RenderPass* m_renderPass = nullptr;

}; // class SkyboxPipeline

} // namespace vox::gfx