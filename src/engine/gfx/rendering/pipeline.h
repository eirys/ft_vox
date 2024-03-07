/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:26:02 by etran             #+#    #+#             */
/*   Updated: 2024/03/07 14:32:55 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vulkan/vulkan.h>

#include "pipeline_decl.h"

namespace vox::gfx {

class Device;
class IPipelineRenderInfo;
class ICommandBuffer;
class RenderPass;
struct RenderPassInfo;

class Pipeline {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    virtual ~Pipeline() = default;

    /* ====================================================================== */

    virtual void    init(const Device& device, const RenderPassInfo& info) = 0;
    virtual void    destroy(const Device& device) = 0;
    virtual void    assemble(const Device& device, const VkPipelineLayout& pipelineLayout) = 0;

    virtual void    record(
        const VkPipelineLayout layout,
        const ICommandBuffer* cmdBuffer,
        const IPipelineRenderInfo& drawInfo) = 0;

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

    VkPipelineShaderStageCreateInfo  _loadShader(
        const Device& device,
        const ShaderType shaderType,
        const char* binPath) const;

}; // class Pipeline

} // namespace vox::gfx
