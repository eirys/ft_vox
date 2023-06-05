/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 13:57:57 by etran             #+#    #+#             */
/*   Updated: 2023/06/06 00:08:05 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "player.h"

namespace vox {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

Player::Player(const scop::Vect3& pos, const scop::Vect3& dir):
	Character(pos, dir, VOX_PLAYER_SPEED) {}

/* ========================================================================== */

void	Player::reset(const scop::Vect3& pos, const scop::Vect3& dir) {
	Character::resetPositionX(pos.x);
	Character::resetPositionY(pos.y + Player::height);
	Character::resetPositionZ(pos.z);
	Character::resetEyeDir(dir);
}

} // namespace vox