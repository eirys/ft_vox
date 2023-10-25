/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 09:50:49 by etran             #+#    #+#             */
/*   Updated: 2023/09/22 16:24:29 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "character.h"

# define PLAYER_FOV		::scop::math::radians(70.0f)
# define CAMERA_ZNEAR	0.1f
# define CAMERA_ZFAR	1000.0f
# define WINDOW_RATIO	(1200.0f / 600.0f)

namespace vox {

class Player: public Character {
public:
	/* ========================================================================= */
	/*                               CONST MEMBERS                               */
	/* ========================================================================= */

	static constexpr const float	height = 1.8f;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	Player(const scop::Vect3& position);

	Player() = default;
	Player(Player&& other) = default;
	Player&	operator=(Player&& other) = default;
	~Player() = default;

	Player(const Player& other) = delete;
	Player&	operator=(const Player& other) = delete;

	/* INHERITED =============================================================== */

	using Character::move;
	using Character::updateEyeDir;

	using Character::getPosition;
	using Character::getCamera;
	using Character::getEyeDir;

	/* ========================================================================= */

	void	reset(const scop::Vect3& pos);

}; // class Player

} // namespace vox