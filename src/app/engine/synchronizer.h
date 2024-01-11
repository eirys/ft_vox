/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   synchronizer.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 21:20:17 by etran             #+#    #+#             */
/*   Updated: 2024/01/07 23:52:35 by etran            ###   ########.fr       */
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

	struct Semaphores {
		gfx::GfxSemaphore		image_available;
		gfx::GfxSemaphore		render_finished;
		gfx::GfxSemaphore		compute_finished;
	};

	struct Fences {
		gfx::Fence				graphics_in_flight;
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
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	Semaphores					_semaphores;
	Fences						_fences;

}; // class Synchronizer

} // namespace scop
