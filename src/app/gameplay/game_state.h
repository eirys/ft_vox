/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameplay.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 09:49:39 by etran             #+#    #+#             */
/*   Updated: 2023/08/12 23:04:55 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "player.h"

namespace vox {

class GameState {
public:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	GameState();

	~GameState() = default;

	GameState(GameState&& other) = delete;
	GameState(const GameState& other) = delete;
	GameState&	operator=(GameState&& other) = delete;
	GameState&	operator=(const GameState& other) = delete;

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

}; // class GameState

} // namespace vox