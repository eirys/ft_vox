/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   synchronizer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 21:37:56 by etran             #+#    #+#             */
/*   Updated: 2024/01/31 16:34:04 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "synchronizer.h"
#include "device.h"

namespace scop {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void	Synchronizer::init(core::Device& device) {
	_semaphores.compute_finished.init(device);
	_semaphores.graphics_finished.init(device);
	_semaphores.image_available.init(device);
	_semaphores.render_finished.init(device);

	_fences.graphics_in_flight.init(device);
	_fences.compute_in_flight.init(device);

	_semaphores.graphics_finished.signal(device.getGraphicsQueue());
}

void	Synchronizer::destroy(core::Device& device) {
	_semaphores.compute_finished.destroy(device);
	_semaphores.graphics_finished.destroy(device);
	_semaphores.image_available.destroy(device);
	_semaphores.render_finished.destroy(device);

	_fences.graphics_in_flight.destroy(device);
	_fences.compute_in_flight.destroy(device);
}

/* ========================================================================== */

Synchronizer::Semaphores&	Synchronizer::getSemaphores() noexcept {
	return _semaphores;
}

const Synchronizer::Semaphores&	Synchronizer::getSemaphores() const noexcept {
	return _semaphores;
}

Synchronizer::Fences&		Synchronizer::getFences() noexcept {
	return _fences;
}

const Synchronizer::Fences&		Synchronizer::getFences() const noexcept {
	return _fences;
}

} // namespace scop