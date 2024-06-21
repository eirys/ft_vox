/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ssao_blur_pipeline.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 22:42:27 by etran             #+#    #+#             */
/*   Updated: 2024/06/17 12:31:30 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "pipeline.h"

namespace game {
class GameState;
}

namespace vox::gfx {

class ICommandBuffer;

class SSAOBlurPipeline final: public Pipeline {
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
        Fragment,

        Count
    };

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    SSAOBlurPipeline() = default;
    ~SSAOBlurPipeline() = default;

    SSAOBlurPipeline(SSAOBlurPipeline&& other) = delete;
    SSAOBlurPipeline(const SSAOBlurPipeline& other) = delete;
    SSAOBlurPipeline& operator=(SSAOBlurPipeline&& other) = delete;
    SSAOBlurPipeline& operator=(const SSAOBlurPipeline& other) = delete;

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

}; // class SSAOBlurPipeline

} // namespace vox::gfx