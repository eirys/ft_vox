/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ssao_pipeline.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 11:53:08 by etran             #+#    #+#             */
/*   Updated: 2024/06/19 12:59:39 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "pipeline.h"

namespace game {
class GameState;
} // namespace game

namespace vox::gfx {

class ICommandBuffer;

class SSAOPipeline final: public Pipeline {
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

    SSAOPipeline() = default;
    ~SSAOPipeline() = default;

    SSAOPipeline(SSAOPipeline&& other) = delete;
    SSAOPipeline(const SSAOPipeline& other) = delete;
    SSAOPipeline& operator=(SSAOPipeline&& other) = delete;
    SSAOPipeline& operator=(const SSAOPipeline& other) = delete;

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

}; // class SSAOPipeline

} // namespace vox::gfx