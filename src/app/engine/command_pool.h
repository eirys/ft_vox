/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_pool.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:14:01 by etran             #+#    #+#             */
/*   Updated: 2023/07/04 09:42:54 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# ifndef GLFW_INCLUDE_VULKAN
#  define GLFW_INCLUDE_VULKAN
# endif

# include <GLFW/glfw3.h>

namespace scop {
namespace graphics {
class Device;

/**
 * @brief Simple wrapper class for VkCommandPool.
*/
class CommandPool {
public:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	CommandPool() = default;
	~CommandPool() = default;

	CommandPool(CommandPool&& other) = delete;
	CommandPool(const CommandPool& other) = delete;
	CommandPool& operator=(CommandPool&& other) = delete;
	CommandPool& operator=(const CommandPool& other) = delete;

	/* ========================================================================= */

	void				init(Device& device);
	void				destroy(Device& device);

	/* ========================================================================= */

	VkCommandPool		getPool() const noexcept;
	operator VkCommandPool() const noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	VkCommandPool		_pool;

}; // class CommandPool

} // namespace graphics
} // namespace scop

