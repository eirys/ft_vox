/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 17:27:51 by etran             #+#    #+#             */
/*   Updated: 2024/01/08 14:46:04 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.h"

#ifndef GLFW_INCLUDE_VULKAN
# define GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>

#include <cstring>
#include <stdexcept> // std::runtime_error

#include "debug.h"

namespace scop::core {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void	Core::init(scop::Window& window) {
	_createInstance();
	_debug_module.init(_vk_instance);
	_device.init(window, _vk_instance);
}

void	Core::destroy() {
	_device.destroy(_vk_instance);
	_debug_module.destroy(_vk_instance);
	vkDestroyInstance(_vk_instance, nullptr);
}

/* ========================================================================== */

Device&	Core::getDevice() noexcept {
	return _device;
}

DebugModule& Core::getDebugModule() noexcept {
	return _debug_module;
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

/**
 * Create a Vulkan instance
*/
void	Core::_createInstance() {
	// Check if validation layers are available
	if (DebugModule::enable_validation_layers && !_checkValidationLayerSupport())
		throw std::runtime_error("validation layers not available");

	// Provides information to driver
	VkApplicationInfo	app_info{};
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pApplicationName = "Ft_Vox";
	app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.pEngineName = "No engine";
	app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.apiVersion = VK_API_VERSION_1_0;

	// Pass those informations to the Vulkan driver
	VkInstanceCreateInfo	instance_info{};
	instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instance_info.pApplicationInfo = &app_info;

	std::vector<const char*>	extensions = _getRequiredExtensions();
	instance_info.enabledExtensionCount = static_cast<uint32_t>(
		extensions.size()
	);
	instance_info.ppEnabledExtensionNames = extensions.data();

	// Setup debug messenger to go along with the instance
	VkDebugUtilsMessengerCreateInfoEXT	debug_info{};
	if (DebugModule::enable_validation_layers) {
		instance_info.enabledLayerCount = static_cast<uint32_t>(
			DebugModule::validation_layers.size()
		);
		instance_info.ppEnabledLayerNames = DebugModule::validation_layers.data();
		_debug_module.populate(debug_info);
		instance_info.pNext =
			(VkDebugUtilsMessengerCreateInfoEXT*)&debug_info;
	} else {
		instance_info.enabledLayerCount = 0;
		instance_info.pNext = nullptr;
	}

	// Create the instance
	if (vkCreateInstance(&instance_info, nullptr, &_vk_instance) != VK_SUCCESS)
		throw std::runtime_error("failed to create _vk_instance");
}


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

} // namespace scop::core