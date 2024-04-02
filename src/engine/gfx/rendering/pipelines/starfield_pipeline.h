/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   starfield_pipeline.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 13:53:07 by etran             #+#    #+#             */
/*   Updated: 2024/03/27 14:29:13 by etran            ###   ########.fr       */
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
        Vertex = 0,
        Fragment,

        First = Vertex,
        Last = Fragment
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

}; // class StarfieldPipeline

} // namespace vox::gfx