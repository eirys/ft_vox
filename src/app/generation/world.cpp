/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 17:42:36 by etran             #+#    #+#             */
/*   Updated: 2023/09/18 17:44:12 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "world.h"

namespace vox {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

World::World(
	const PerlinNoise& noise,
	std::size_t width,
	std::size_t height
) {
	_origin.x = width / 2;
	_origin.z = height / 2;
	_origin.y = noise.noiseAt(_origin.x, _origin.z) + BLOCK_SIZE;
}

/* ========================================================================== */

void	World::setOrigin(const scop::Vect3& origin) noexcept {
	_origin = origin;
}

const scop::Vect3&	World::getOrigin() const noexcept {
	return _origin;
}

} // namespace vox