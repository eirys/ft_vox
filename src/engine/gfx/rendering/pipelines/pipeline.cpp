/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 22:13:57 by etran             #+#    #+#             */
/*   Updated: 2024/06/16 15:43:09 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipeline.h"
#include "device.h"
#include "io_helpers.h"

#include <string>
#include <stdexcept>

#include "debug.h"

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

VkPipeline Pipeline::getPipeline() const noexcept {
    return m_pipeline;
}

const PipelineLayout& Pipeline::getPipelineLayout() const noexcept {
    return *m_pipelineLayout;
}

/* ========================================================================== */
/*                                  PROTECTED                                 */
/* ========================================================================== */

VkShaderModule Pipeline::_createShaderModule(const Device& device, const char* binPath) const {
    const std::vector<u8>   bin = io::readBinary(binPath);

    VkShaderModuleCreateInfo moduleInfo{};
    moduleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    moduleInfo.codeSize = bin.size();
    moduleInfo.pCode = reinterpret_cast<const u32*>(bin.data());

    VkShaderModule          shaderModule;
    if (vkCreateShaderModule(device.getDevice(), &moduleInfo, nullptr, &shaderModule) != VK_SUCCESS)
        throw std::runtime_error("failed to create shader module");

    return shaderModule;
}

} // namespace vox::gfx