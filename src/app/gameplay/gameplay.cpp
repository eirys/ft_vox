/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameplay.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 15:35:50 by etran             #+#    #+#             */
/*   Updated: 2023/07/04 10:25:26 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gameplay.h"

namespace vox {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

Gameplay::Gameplay():
	_player(scop::Vect3(), VOX_DEFAULT_EYE_DIR) {}

/* ========================================================================== */

void	Gameplay::reset() noexcept {
	_player.reset(_world_origin, {0.0f, 0.0f, 1.0f});
}

void	Gameplay::setOrigin(const scop::Vect3& origin) noexcept {
	_world_origin = origin;
}

void	Gameplay::setCameraDir(float yaw, float pitch) noexcept {
	_player.updateEyeDir(yaw, pitch);
}

void	Gameplay::setCameraPos(const scop::Vect3& new_pos) noexcept {
	_player.move(new_pos);
}

/* ========================================================================== */

const Player&	Gameplay::getPlayer() const noexcept {
	return _player;
}

Player&	Gameplay::getPlayer() noexcept {
	return _player;
}

} // namespace vox