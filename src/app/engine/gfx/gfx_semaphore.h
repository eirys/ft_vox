/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gfx_semaphore.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 21:26:18 by etran             #+#    #+#             */
/*   Updated: 2024/01/31 14:43:50 by etran            ###   ########.fr       */
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
 * @brief Wrapper for VkSemaphore. Synchronizes GPU commands.
*/
class GfxSemaphore {
public:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	GfxSemaphore() = default;
	~GfxSemaphore() = default;

	GfxSemaphore(GfxSemaphore&& x) = delete;
	GfxSemaphore(const GfxSemaphore& x) = delete;
	GfxSemaphore& operator=(GfxSemaphore&& rhs) = delete;
	GfxSemaphore& operator=(const GfxSemaphore& rhs) = delete;

	/* ========================================================================= */

	void		init(scop::core::Device& device);
	void		destroy(scop::core::Device& device);


	void		signal(VkQueue queue);

	/* ========================================================================= */

	VkSemaphore	getSemaphore() const noexcept;

private:
	/* ========================================================================= */
	/*                                    DATA                                   */
	/* ========================================================================= */

	VkSemaphore	_semaphore;

}; // class GfxSemaphore

} // namespace scop::gfx