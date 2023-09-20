/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 09:50:49 by etran             #+#    #+#             */
/*   Updated: 2023/09/18 11:08:39 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "character.h"

# ifdef __LINUX
#  define VOX_CAMERA_SPEED		0.1f
# else
#  define VOX_CAMERA_SPEED		0.05f
# endif
# define VOX_DEFAULT_EYE_DIR	scop::Vect3(0.0f, 0.0f, 1.0f)

# define VOX_DEFAULT_POS_X		1.0f
# define VOX_DEFAULT_POS_Y		2.0f
# define VOX_DEFAULT_POS_Z		3.0f

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

	Player(const scop::Vect3& position, const scop::Vect3& eye_dir);

	~Player() = default;

	Player() = delete;
	Player(Player&& other) = delete;
	Player(const Player& other) = delete;
	Player&	operator=(Player&& other) = delete;
	Player&	operator=(const Player& other) = delete;

	/* INHERITED =============================================================== */

	using Character::move;
	using Character::updateEyeDir;

	using Character::getPosition;
	using Character::getCamera;
	using Character::getEyeDir;

	/* ========================================================================= */

	void	reset(const scop::Vect3& pos, const scop::Vect3& eye_dir);

}; // class Player

} // namespace vox