/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 13:57:57 by etran             #+#    #+#             */
/*   Updated: 2024/01/08 15:18:05 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "player.h"

#include "debug.h"
namespace vox {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

Player::Player(const scop::Vect3& pos/* , const scop::Vect3& eye_dir */):
	Character(pos, VOX_DEFAULT_EYE_DIR, VOX_CAMERA_SPEED) {}

/* ========================================================================== */

void	Player::reset(const scop::Vect3& pos/* , const scop::Vect3& eye_dir */) {
	SCOP_DEBUG("Character::reset: " << pos);
	Character::resetPositionX(pos.x);
	Character::resetPositionY(pos.y + Player::height);
	Character::resetPositionZ(pos.z);
	Character::resetEyeDir(VOX_DEFAULT_EYE_DIR);
}

} // namespace vox