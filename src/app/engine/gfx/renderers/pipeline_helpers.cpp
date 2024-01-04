/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_helpers.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 22:48:05 by etran             #+#    #+#             */
/*   Updated: 2024/01/01 16:21:22 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipeline_helpers.h"
#include "device.h"
#include "utils.h"

#ifndef SHD_BIN_DIR
# define SHD_BIN_DIR ""
#endif

namespace scop::gfx {

/* ========================================================================== */
/*                                  PROTECTED                                 */
/* ========================================================================== */

/**
 * @brief Create a shader module from a file
*/
VkShaderModule	createShaderModule(
	scop::core::Device& device,
	const std::string& path
) {
	// Read compiled shader binary file
	std::vector<uint8_t>		code = scop::utils::readFile(SHD_BIN_DIR + path);
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

} // namespace scop::gfx