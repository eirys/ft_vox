/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameplay.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 15:35:50 by etran             #+#    #+#             */
/*   Updated: 2023/08/12 23:04:49 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gameplay.h"

namespace vox {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

GameState::GameState():
	_player(scop::Vect3(), VOX_DEFAULT_EYE_DIR) {}

/* ========================================================================== */

void	GameState::reset() noexcept {
	_player.reset(_world_origin, {0.0f, 0.0f, 1.0f});
}

void	GameState::setOrigin(const scop::Vect3& origin) noexcept {
	_world_origin = origin;
}

void	GameState::setCameraDir(float yaw, float pitch) noexcept {
	_player.updateEyeDir(yaw, pitch);
}

void	GameState::setCameraPos(const scop::Vect3& new_pos) noexcept {
	_player.move(new_pos);
}

/* ========================================================================== */

const Player&	GameState::getPlayer() const noexcept {
	return _player;
}

Player&	GameState::getPlayer() noexcept {
	return _player;
}

} // namespace vox