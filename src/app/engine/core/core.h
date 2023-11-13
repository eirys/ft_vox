/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 17:26:43 by etran             #+#    #+#             */
/*   Updated: 2023/11/10 17:36:49 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# include <vulkan/vulkan.h>

namespace std {
template <typename T> class vector;
}

namespace scop::core {

class DebugModule;
class Device;

class Core final {
public:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	Core() = default;
	~Core() = default;

	Core(Core&& other) = delete;
	Core(const Core& other) = delete;
	Core& operator=(Core&& other) = delete;
	Core& operator=(const Core& other) = delete;

	/* ========================================================================= */

	void				init(Window& window, const GameState& game);
	void				destroy();

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	VkInstance			_vk_instance;

	DebugModule			_debug_module;
	Device				_device;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	bool						_checkValidationLayerSupport();
	std::vector<const char*>	_getRequiredExtensions();

}; // class Core

} // namespace scop::core
