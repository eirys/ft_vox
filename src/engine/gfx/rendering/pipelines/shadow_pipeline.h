/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadow_pipeline.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:05:50 by etran             #+#    #+#             */
/*   Updated: 2024/06/16 15:14:01 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "pipeline.h"

namespace vox::gfx {

class ShadowRenderPass;

class ShadowPipeline final: public Pipeline {
public:
    /* ====================================================================== */
    /*                                TYPEDEFS                                */
    /* ====================================================================== */

    using super = Pipeline;

    /* ====================================================================== */
    /*                                  ENUMS                                 */
    /* ====================================================================== */

    enum class ShaderStage: u32 {
        Vertex,

        Count
    };

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    ShadowPipeline() = default;
    ~ShadowPipeline() = default;

    ShadowPipeline(ShadowPipeline&& other) = delete;
    ShadowPipeline(const ShadowPipeline& other) = delete;
    ShadowPipeline& operator=(ShadowPipeline&& other) = delete;
    ShadowPipeline& operator=(const ShadowPipeline& other) = delete;

    /* ====================================================================== */

    void    init(
        const Device& device,
        const VkRenderPass& renderPass,
        const PipelineLayout& pipelineLayout) override;
    void    destroy(const Device& device) override;

    void    record(const ICommandBuffer* cmdBuffer) const override;

private:
    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static constexpr u32    SHADER_STAGE_COUNT = (u32)ShaderStage::Count;

}; // class ShadowPipeline

} // namespace vox::gfx