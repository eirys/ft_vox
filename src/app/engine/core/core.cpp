/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 17:27:51 by etran             #+#    #+#             */
/*   Updated: 2023/11/10 17:29:52 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.h"

namespace vox {

/* ========================================================================== */

/**
 * Check if all required extensions are available for validation layers
*/
bool	Core::_checkValidationLayerSupport() {
	uint32_t	layer_count;
	vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

	std::vector<VkLayerProperties>	available_layers(layer_count);
	vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

	bool	supported = false;
	for (const char* layer_name: DebugModule::validation_layers) {
		for (const VkLayerProperties& layer_properties: available_layers) {
			if (!strcmp(layer_name, layer_properties.layerName)) {
				supported = true;
				break;
			}
		}
	}
	return supported;
}

/**
 *  Retrieve list of extensions if validation layers enabled
 */
std::vector<const char*>	Core::_getRequiredExtensions() {
	uint32_t		glfw_extension_count = 0;
	const char**	glfw_extensions;
	glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

	std::vector<const char*>	extensions(
		glfw_extensions,
		glfw_extensions + glfw_extension_count
	);

	if (DebugModule::enable_validation_layers) {
		extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
	return extensions;
}

} // namespace vox