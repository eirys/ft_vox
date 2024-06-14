/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deferred_pipeline.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:04:18 by etran             #+#    #+#             */
/*   Updated: 2024/06/14 18:48:42 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "pipeline.h"

namespace game {
class GameState;
} // namespace game

namespace vox::gfx {

class ICommandBuffer;

class DeferredPipeline final: public Pipeline {
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

    DeferredPipeline() = default;
    ~DeferredPipeline() = default;

    DeferredPipeline(DeferredPipeline&& other) = delete;
    DeferredPipeline(const DeferredPipeline& other) = delete;
    DeferredPipeline& operator=(DeferredPipeline&& other) = delete;
    DeferredPipeline& operator=(const DeferredPipeline& other) = delete;

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

    static constexpr u32    SHADER_STAGE_COUNT = (u32)ShaderStage::Count;

}; // class DeferredPipeline

} // namespace vox::gfx