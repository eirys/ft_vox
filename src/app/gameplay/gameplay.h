/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameplay.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 09:49:39 by etran             #+#    #+#             */
/*   Updated: 2023/07/04 10:24:47 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "player.h"

namespace vox {

class Gameplay {
public:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	Gameplay();

	~Gameplay() = default;

	Gameplay(Gameplay&& other) = delete;
	Gameplay(const Gameplay& other) = delete;
	Gameplay&	operator=(Gameplay&& other) = delete;
	Gameplay&	operator=(const Gameplay& other) = delete;

	/* ========================================================================= */

	void			reset() noexcept;
	void			setOrigin(const scop::Vect3& origin) noexcept;
	void			setCameraDir(float yaw, float pitch) noexcept;
	void			setCameraPos(const scop::Vect3& new_pos) noexcept;

	/* ========================================================================= */

	const Player&	getPlayer() const noexcept;
	Player&			getPlayer() noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	scop::Vect3		_world_origin = {0.0f, 0.0f, 0.0f};
	Player			_player;

}; // class Gameplay

} // namespace vox