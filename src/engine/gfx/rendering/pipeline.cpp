/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 22:13:57 by etran             #+#    #+#             */
/*   Updated: 2024/03/07 14:28:04 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipeline.h"
#include "device.h"
#include "io_helpers.h"

#include <string>
#include <stdexcept>

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

VkPipeline Pipeline::getPipeline() const noexcept {
    return m_pipeline;
}

/* ========================================================================== */
/*                                  PROTECTED                                 */
/* ========================================================================== */

VkPipelineShaderStageCreateInfo Pipeline::_loadShader(
    const Device& device,
    const ShaderType shaderType,
    const char* binPath
) const {
    const std::vector<u8>   bin = io::readBinary(binPath);
    VkShaderModule          shaderModule;

    VkShaderModuleCreateInfo moduleInfo{};
    moduleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    moduleInfo.codeSize = bin.size();
    moduleInfo.pCode = (u32*)bin.data();

    if (vkCreateShaderModule(device.getDevice(), &moduleInfo, nullptr, &shaderModule) != VK_SUCCESS)
        throw std::runtime_error("failed to create shader module");

    VkPipelineShaderStageCreateInfo shaderStageInfo{};
    shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageInfo.stage = (VkShaderStageFlagBits)shaderType;
    shaderStageInfo.module = shaderModule;
    shaderStageInfo.pName = "main";

    return shaderStageInfo;
}

} // namespace vox::gfx