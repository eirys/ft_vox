/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_state.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 15:35:50 by etran             #+#    #+#             */
/*   Updated: 2023/08/20 02:44:26 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game_state.h"

namespace vox {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

GameState::GameState():
	_player(scop::Vect3(), VOX_DEFAULT_EYE_DIR) {}

/* ========================================================================== */

void	GameState::reset() noexcept {
	_player.reset(_world.getOrigin(), VOX_DEFAULT_EYE_DIR);
}

/**
 * @brief Sets spawn position.
*/
void	GameState::setOrigin(const scop::Vect3& origin) noexcept {
	_world.getOrigin() = origin;
}

void	GameState::setCameraDir(float yaw, float pitch) noexcept {
	_player.updateEyeDir(yaw, pitch);
}

void	GameState::setCameraPos(const scop::Vect3& new_pos) noexcept {
	_player.move(new_pos);
}

/* ========================================================================== */

const World&	GameState::getWorld() const noexcept {
	return _world;
}

World&	GameState::getWorld() noexcept {
	return _world;
}

const Player&	GameState::getPlayer() const noexcept {
	return _player;
}

Player&	GameState::getPlayer() noexcept {
	return _player;
}

} // namespace vox