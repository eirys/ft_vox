/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 17:59:15 by etran             #+#    #+#             */
/*   Updated: 2023/08/20 10:15:58 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipeline.h"
#include "device.h"
#include "utils.h"
#include "descriptor_set.h"

#include <stdexcept> // std::runtime_error

namespace scop::graphics {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void	Pipeline::destroy(Device& device) {
	_descriptor->destroy(device);
	_render_pass->destroy(device);
	_target->destroy(device);
	_texture->destroy(device);
	vkDestroyPipeline(device.getLogicalDevice(), _pipeline, nullptr);
}

void	Pipeline::update(const UniformBufferObject& ubo) noexcept {
	(void)ubo;
}

/* ========================================================================== */

VkPipeline	Pipeline::getPipeline() const noexcept {
	return _pipeline;
}

Pipeline::RenderPassPtr	Pipeline::getRenderPass() const noexcept {
	return _render_pass;
}

Pipeline::TextureHandlerPtr	Pipeline::getTextureHandler() const noexcept {
	return _texture;
}

Pipeline::TargetPtr	Pipeline::getTarget() const noexcept {
	return _target;
}

Pipeline::DescriptorSetPtr	Pipeline::getDescriptor() const noexcept {
	return _descriptor;
}

/* ========================================================================== */
/*                                  PROTECTED                                 */
/* ========================================================================== */

/**
 * @brief Create a shader module from a file
*/
VkShaderModule	Pipeline::_createShaderModule(
	Device& device,
	const std::string& path
) {
	// Read compiled shader binary file
	std::vector<uint8_t>		code = ::scop::utils::readFile(path);
	VkShaderModuleCreateInfo	shader_info{};

	shader_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shader_info.codeSize = code.size();
	shader_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule	shader_module;
	if (vkCreateShaderModule(device.getLogicalDevice(), &shader_info, nullptr, &shader_module) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shader module");
	}
	return shader_module;
}

} // namespace scop::graphics