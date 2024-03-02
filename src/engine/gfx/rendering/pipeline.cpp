/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 22:13:57 by etran             #+#    #+#             */
/*   Updated: 2024/02/29 22:30:59 by etran            ###   ########.fr       */
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

VkShaderModule Pipeline::_createShaderModule(const Device& device, const char* binPath) const {
    const std::vector<u8>   bin = io::readBinary(binPath);
    VkShaderModule          shaderModule;

    VkShaderModuleCreateInfo moduleInfo = {};
    moduleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    moduleInfo.codeSize = bin.size();
    moduleInfo.pCode = (u32*)bin.data();

    if (vkCreateShaderModule(device.getDevice(), &moduleInfo, nullptr, &shaderModule) != VK_SUCCESS)
        throw std::runtime_error("failed to create shader module");
    return shaderModule;
}

} // namespace vox::gfx