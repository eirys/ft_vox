/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clouds_pipeline.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 01:29:34 by etran             #+#    #+#             */
/*   Updated: 2024/05/28 14:34:14 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "pipeline.h"

namespace vox::gfx {

class CloudsRenderPass;

class CloudsPipeline final: public Pipeline {
public:
    /* ====================================================================== */
    /*                                TYPEDEFS                                */
    /* ====================================================================== */

    using super = Pipeline;

    /* ====================================================================== */
    /*                                  ENUMS                                 */
    /* ====================================================================== */

    enum class ShaderStage: u32 {
        VertexInstance = 0,
        Fragment,

        First = VertexInstance,
        Last = Fragment
    };

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    CloudsPipeline();
    ~CloudsPipeline();

    CloudsPipeline(CloudsPipeline&& other) = delete;
    CloudsPipeline(const CloudsPipeline& other) = delete;
    CloudsPipeline& operator=(CloudsPipeline&& other) = delete;
    CloudsPipeline& operator=(const CloudsPipeline& other) = delete;

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

private:
    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static constexpr u32    SHADER_STAGE_COUNT = static_cast<u32>(ShaderStage::Last) + 1;

}; // class CloudsPipeline

} // namespace vox::gfx
