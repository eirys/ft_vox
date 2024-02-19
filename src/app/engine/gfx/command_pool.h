/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_pool.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:14:01 by etran             #+#    #+#             */
/*   Updated: 2024/01/31 12:19:14 by etran            ###   ########.fr       */
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

	static CommandBuffer	createCommandBuffer(
		scop::core::Device& device,
		CommandBufferType type);
	static void				destroyBuffer(
		scop::core::Device& device,
		CommandBuffer& buffer);

	/* ========================================================================= */

	static VkCommandPool	getDrawPool() noexcept;
	static VkCommandPool	getComputePool() noexcept;

private:
	/* ========================================================================= */
	/*                                    DATA                                   */
	/* ========================================================================= */

	static VkCommandPool	_draw_pool;
	static VkCommandPool	_compute_pool;

}; // class CommandPool

} // namespace scop::gfx