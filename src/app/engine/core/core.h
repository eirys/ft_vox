/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 17:26:43 by etran             #+#    #+#             */
/*   Updated: 2023/11/18 21:23:23 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# include <vulkan/vulkan.h>

// Std
# include <vector>

# include "device.h"
# include "debug_module.h"

namespace scop {
class Window;
} // namespace scop

namespace vox {
class GameState;
}

namespace scop::core {

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

	void						init(scop::Window& window);
	void						destroy();

	/* ========================================================================= */

	Device&						getDevice() noexcept;
	DebugModule&				getDebugModule() noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	VkInstance					_vk_instance;

	DebugModule					_debug_module;
	Device						_device;

	// Synchronizer				_synchronizer;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void						_createInstance();
	bool						_checkValidationLayerSupport();
	std::vector<const char*>	_getRequiredExtensions();

}; // class Core

} // namespace scop::core
