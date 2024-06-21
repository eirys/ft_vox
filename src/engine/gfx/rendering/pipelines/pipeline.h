/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:26:02 by etran             #+#    #+#             */
/*   Updated: 2024/06/16 16:57:42 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vulkan/vulkan.h>
#include "types.h"
#include "enum.h"

namespace vox::gfx {

enum class ShaderType: u8 {
    VS = VK_SHADER_STAGE_VERTEX_BIT,
    FS = VK_SHADER_STAGE_FRAGMENT_BIT,
    CS = VK_SHADER_STAGE_COMPUTE_BIT
};

class Device;
class ICommandBuffer;
class PipelineLayout;

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
        const PipelineLayout& pipelineLayout) = 0;
    virtual void    destroy(const Device& device) = 0;

    virtual void    record(const ICommandBuffer* cmdBuffer) const = 0;

    VkPipeline              getPipeline() const noexcept;
    const PipelineLayout&   getPipelineLayout() const noexcept;

protected:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    VkPipeline              m_pipeline = VK_NULL_HANDLE;
    const PipelineLayout*   m_pipelineLayout = nullptr;

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
