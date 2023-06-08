/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:17:06 by etran             #+#    #+#             */
/*   Updated: 2023/06/08 11:36:19 by etran            ###   ########.fr       */
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
# include "utils.h"
# include "matrix.h"
# include "vertex.h"
# include "image_handler.h"
# include "engine.h"
# include "uniform_buffer_object.h"
# include "gameplay.h"
# include "material.h"
# include "timer.h"

# define SCOP_MOUSE_SENSITIVITY	0.25f
# define SCOP_MOVE_SPEED		0.05f

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
	App& operator=(const App& rhs) = delete;

	/* MAIN FUNCTION =========================================================== */

	void							run();

	/* ========================================================================= */

	void							resetGame() noexcept;
	void							toggleMove(ObjectDirection dir) noexcept;
	void							untoggleMove(ObjectDirection dir) noexcept;
	void							updateCameraDir(float x, float y) noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	vox::Gameplay					game;
	scop::Window					window;
	scop::graphics::Engine			engine;

	std::vector<scop::Vertex>		vertices;
	std::vector<uint32_t>			indices;
	std::unique_ptr<scop::Image>	image;
	UniformBufferObject::Light		light;

	bool							keys_pressed_directions[6] = { false };
	scop::Vect3						movement{};

	Timer							timer;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void							loadModel(const std::string& path);
	void							loadTerrain();
	void							loadLight(
		const scop::mtl::Material& mat = scop::mtl::Material()
	);
	void							loadTexture(
		std::unique_ptr<scop::Image>&& image = nullptr
	);
	void							updateGame();

}; // class App

} // namespace scop