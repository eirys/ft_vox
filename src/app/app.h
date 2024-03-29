/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:17:06 by etran             #+#    #+#             */
/*   Updated: 2023/11/17 00:09:37 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# include <vulkan/vulkan.h>

# include "game_state.h"
# include "window.h"
# include "engine.h"
# include "timer.h"

# ifdef __LINUX
#  define SCOP_MOVE_SPEED		0.2f
# else
#  define SCOP_MOVE_SPEED		0.05f
# endif

namespace scop {

enum ObjectDirection {
	MOVE_FORWARD = 0,
	MOVE_BACKWARD = 1,
	MOVE_LEFT = 2,
	MOVE_RIGHT = 3,
	MOVE_UP = 4,
	MOVE_DOWN = 5
};

/**
 * @brief Main class of the program.
*/
class App {
public:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	App();
	~App();

	App(const App& x) = delete;
	App(App&& x) = delete;
	App& operator=(App&& rhs) = delete;
	App& operator=(const App& rhs) = delete;

	/* MAIN FUNCTION =========================================================== */

	void						run();

	/* ========================================================================= */

	void						resetGame() noexcept;
	void						toggleMove(ObjectDirection dir) noexcept;
	void						untoggleMove(ObjectDirection dir) noexcept;
	void						updateCameraDir(float x, float y) noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	// TODO: Put to engine
	vox::GameState				_game;
	scop::Window				_window;
	scop::Engine				_engine;
	Timer						_timer;

	// Camera input
	bool						_keys_pressed_directions[6] = { false };
	scop::Vect3					_movement{};

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void						_updateGame();

}; // class App

} // namespace scop