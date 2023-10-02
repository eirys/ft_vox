/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:17:06 by etran             #+#    #+#             */
/*   Updated: 2023/09/29 16:54:54 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# ifndef GLFW_INCLUDE_VULKAN
#  define GLFW_INCLUDE_VULKAN
# endif

// Window handler
# include <GLFW/glfw3.h>

// Std
# include <memory> // std::unique_ptr
# include <map> // std::map

# include "window.h"
# include "vertex.h"
# include "engine.h"
# include "game_state.h"
# include "material.h"
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
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	typedef	graphics::TextureHandler::Texture	Texture;

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

	vox::GameState				_game;
	scop::Window				_window;
	scop::graphics::Engine		_engine;
	Timer						_timer;

	// Resources
	// std::vector<vox::Vertex>	_vertices;
	// std::vector<uint32_t>		_indices;
	// std::vector<Texture>		_textures;

	// Camera input
	bool						_keys_pressed_directions[6] = { false };
	scop::Vect3					_movement{};

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	//void						_loadModel(const std::string& path);
	void						_loadTerrain();
	void						_updateGame();

}; // class App

} // namespace scop