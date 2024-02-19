/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   synchronizer.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 21:20:17 by etran             #+#    #+#             */
/*   Updated: 2024/02/02 23:11:12 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# include <vulkan/vulkan.h>

#include "gfx_semaphore.h"
#include "fence.h"

namespace scop {

/**
 * @brief Manager for synchronization objects (VkSemaphores, VkFences, mutexes).
*/
class Synchronizer {
public:
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	// Semaphores, used for synchronization between GPU commands.
	struct Semaphores {
		// Signals that an image is available for rendering.
		gfx::GfxSemaphore		image_available;

		// Signals that rendering has finished.
		gfx::GfxSemaphore		render_finished;

		// Signals that compute command has finished.
		gfx::GfxSemaphore		compute_finished;

		// Signals that graphics command has finished.
		gfx::GfxSemaphore		graphics_finished;
	};

	// Fences, used for synchronization between CPU and GPU.
	struct Fences {
		// Signals that graphics command is ongoing.
		gfx::Fence				graphics_in_flight;

		// Signals that compute command is ongoing.
		gfx::Fence				compute_in_flight;
	};

	// TODO
	struct Mutexes {

	};

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	Synchronizer() = default;
	~Synchronizer() = default;

	Synchronizer(Synchronizer&& x) = delete;
	Synchronizer(const Synchronizer& x) = delete;
	Synchronizer& operator=(Synchronizer&& rhs) = delete;
	Synchronizer& operator=(const Synchronizer& rhs) = delete;

	/* ========================================================================= */

	void		init(core::Device& device);
	void		destroy(core::Device& device);

	/* ========================================================================= */

	Semaphores&					getSemaphores() noexcept;
	const Semaphores&			getSemaphores() const noexcept;

	Fences&						getFences() noexcept;
	const Fences&				getFences() const noexcept;

private:
	/* ========================================================================= */
	/*                                    DATA                                   */
	/* ========================================================================= */

	Semaphores					_semaphores;
	Fences						_fences;

}; // class Synchronizer

} // namespace scop
