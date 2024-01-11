/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fence.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 21:35:00 by etran             #+#    #+#             */
/*   Updated: 2024/01/07 22:09:38 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# include <vulkan/vulkan.h>

namespace scop::core {
class Device;
}

namespace scop::gfx {

/**
 * @brief Wrapper for VkFence. Synchronizes CPU and GPU.
*/
class Fence {
public:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	Fence() = default;
	~Fence() = default;

	Fence(Fence&& x) = delete;
	Fence(const Fence& x) = delete;
	Fence& operator=(Fence&& rhs) = delete;
	Fence& operator=(const Fence& rhs) = delete;

	/* ========================================================================= */

	void		init(
		const scop::core::Device& device,
		VkFenceCreateFlagBits flags = VK_FENCE_CREATE_SIGNALED_BIT);
	void		destroy(const scop::core::Device& device);

	void		wait(
		const scop::core::Device& device,
		uint64_t timeout = UINT64_MAX) const;
	void		reset(const scop::core::Device& device) const;

	/* ========================================================================= */

	VkFence		getFence() const noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	VkFence	_fence;

}; // class Fence

} // namespace scop::gfx