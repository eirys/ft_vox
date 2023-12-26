/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_state.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 09:49:39 by etran             #+#    #+#             */
/*   Updated: 2023/12/26 17:11:02 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "world.h"
# include "player.h"
# include "clock.h"

namespace vox {

class GameState {
public:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	GameState(std::size_t seed);
	~GameState() = default;

	GameState() = delete;
	GameState(GameState&& other) = delete;
	GameState(const GameState& other) = delete;
	GameState&	operator=(GameState&& other) = delete;
	GameState&	operator=(const GameState& other) = delete;

	/* ========================================================================= */

	void			reset() noexcept;
	void			setOrigin(const scop::Vect3& origin) noexcept;
	void			setCameraDir(float yaw, float pitch) noexcept;
	void			setCameraPos(const scop::Vect3& new_pos) noexcept;

	void			update();
	void			updateWorld();

	/* ========================================================================= */

	const World&	getWorld() const noexcept;
	World&			getWorld() noexcept;
	const Player&	getPlayer() const noexcept;
	Player&			getPlayer() noexcept;
	float			getElapsedTime() const noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	World			_world;
	Player			_player;
	Clock			_clock;

}; // class GameState

} // namespace vox