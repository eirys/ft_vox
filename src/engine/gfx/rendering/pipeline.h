/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:26:02 by etran             #+#    #+#             */
/*   Updated: 2024/03/10 19:14:48 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vulkan/vulkan.h>

#include "pipeline_decl.h"

namespace vox::gfx {

class Device;
class ICommandBuffer;
class RenderPass;
class DescriptorTable;

struct RecordInfo;
struct RenderPassInfo;

class Pipeline {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    virtual ~Pipeline() = default;

    /* ====================================================================== */

    virtual void    init(const Device& device, const RenderPassInfo* info) = 0;
    virtual void    destroy(const Device& device) = 0;
    virtual void    assemble(const Device& device, const VkPipelineLayout& pipelineLayout) = 0;

    virtual void    record(
        const VkPipelineLayout layout,
        const DescriptorTable& descriptorTable,
        const ICommandBuffer* cmdBuffer,
        const RecordInfo& recordInfo) = 0;

    /* ====================================================================== */

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
