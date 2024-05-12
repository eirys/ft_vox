/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:26:02 by etran             #+#    #+#             */
/*   Updated: 2024/04/08 16:41:54 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "pipeline_decl.h"

namespace vox::gfx {

class Device;
class ICommandBuffer;
class DescriptorTable;

class Pipeline {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    virtual ~Pipeline() = default;

    /* ====================================================================== */

    virtual void    init(
        const Device& device,
        const VkRenderPass& renderPass,
        const VkPipelineLayout& pipelineLayout) = 0;
    virtual void    destroy(const Device& device) = 0;

    virtual void    record(
        const VkPipelineLayout layout,
        const DescriptorTable& descriptorTable,
        const ICommandBuffer* cmdBuffer) = 0;

    VkPipeline          getPipeline() const noexcept;

protected:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    VkPipeline      m_pipeline = VK_NULL_HANDLE;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    Pipeline() = default;

    Pipeline(Pipeline&& other) = delete;
    Pipeline(const Pipeline& other) = delete;
    Pipeline& operator=(Pipeline&& other) = delete;
    Pipeline& operator=(const Pipeline& other) = delete;

    /* ====================================================================== */

    VkShaderModule  _createShaderModule(const Device& device, const char* binPath) const;

}; // class Pipeline

} // namespace vox::gfx
