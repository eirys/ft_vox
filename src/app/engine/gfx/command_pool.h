/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_pool.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:14:01 by etran             #+#    #+#             */
/*   Updated: 2023/11/19 11:01:46 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# include <vulkan/vulkan.h>

namespace scop::core {
class Device;
}

namespace scop::gfx {

class CommandBuffer;
enum class CommandBufferType: uint8_t;

/**
 * @brief Simple wrapper class for VkCommandPool.
*/
class CommandPool final {
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

	void					init(scop::core::Device& device);
	void					destroy(scop::core::Device& device);

	static CommandBuffer	createBuffer(
		scop::core::Device& device,
		CommandBufferType type);
	static void				destroyBuffer(
		scop::core::Device& device,
		CommandBuffer& buffer);

	/* ========================================================================= */

	static VkCommandPool	getPool() noexcept;
	operator VkCommandPool() const noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	static VkCommandPool	_draw_pool;
	static VkCommandPool	_compute_pool;

}; // class CommandPool

} // namespace scop::gfx