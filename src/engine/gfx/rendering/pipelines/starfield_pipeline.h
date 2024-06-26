/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   starfield_pipeline.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 13:53:07 by etran             #+#    #+#             */
/*   Updated: 2024/06/16 15:14:01 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "pipeline.h"

namespace vox::gfx {

class StarfieldPipeline final: public Pipeline {
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

    StarfieldPipeline() = default;
    ~StarfieldPipeline() = default;

    StarfieldPipeline(StarfieldPipeline&& other) = delete;
    StarfieldPipeline(const StarfieldPipeline& other) = delete;
    StarfieldPipeline& operator=(StarfieldPipeline&& other) = delete;
    StarfieldPipeline& operator=(const StarfieldPipeline& other) = delete;

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

    static constexpr u32 SHADER_STAGE_COUNT = (u32)ShaderStage::Count;

}; // class StarfieldPipeline

} // namespace vox::gfx
