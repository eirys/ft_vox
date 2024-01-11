/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_module.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:14:08 by etran             #+#    #+#             */
/*   Updated: 2024/01/08 16:47:59 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <vulkan/vulkan.h>

// Std
# include <vector>

namespace scop::core {

class DebugModule final {
public:
	/* ========================================================================= */
	/*                               CONST MEMBERS                               */
	/* ========================================================================= */

	static const std::vector<const char*>	validation_layers;

	#ifdef __RELEASE
	static const constexpr bool			enable_validation_layers = true;
	#else
	static const constexpr bool			enable_validation_layers = false;
	#endif

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	DebugModule() = default;
	~DebugModule() = default;

	DebugModule(DebugModule&& other) = delete;
	DebugModule(const DebugModule& other) = delete;
	DebugModule& operator=(DebugModule&& other) = delete;
	DebugModule& operator=(const DebugModule& other) = delete;

	/* ========================================================================= */

	void		init(VkInstance vk_instance);
	void		destroy(VkInstance vk_instance);
	void		populate(VkDebugUtilsMessengerCreateInfoEXT& create_info);

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	VkDebugUtilsMessengerEXT		debug_messenger;

}; // class DebugModule

/* ========================================================================== */
/*                                    OTHER                                   */
/* ========================================================================== */

VkResult	CreateDebugUtilsMessengerEXT(
	VkInstance instance,
	const VkDebugUtilsMessengerCreateInfoEXT* p_create_info,
	const VkAllocationCallbacks* p_allocator,
	VkDebugUtilsMessengerEXT* p_debug_messenger
);

void	DestroyDebugUtilsMessengerEXT(
	VkInstance instance,
	VkDebugUtilsMessengerEXT debug_messenger,
	const VkAllocationCallbacks* p_allocator
);

VKAPI_ATTR	VkBool32 VKAPI_CALL	debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
	VkDebugUtilsMessageTypeFlagsEXT message_type,
	const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data,
	void* p_user_data
);

} // namespace scop::core