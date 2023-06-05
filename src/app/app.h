/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:17:06 by etran             #+#    #+#             */
/*   Updated: 2023/06/06 00:18:19 by etran            ###   ########.fr       */
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

# define SCOP_MOUSE_SENSITIVITY	0.25f
# define SCOP_MOVE_SPEED		0.05f
# define SCOP_ROTATION_SPEED	0.25f // deg

namespace scop {

enum ObjectDirection {
	MOVE_FORWARD = 0,
	MOVE_BACKWARD = 1,
	MOVE_LEFT = 2,
	MOVE_RIGHT = 3,
	MOVE_UP = 4,
	MOVE_DOWN = 5
};

enum TextureState {
	TEXTURE_GRAYSCALE = 1,
	TEXTURE_COLOR = 2,
	TEXTURE_ENABLED = 0
};

/**
 * @brief Main class of the program.
*/
class App {
public:
	/* ========================================================================= */
	/*                               CONST MEMBERS                               */
	/* ========================================================================= */

	static constexpr float			transition_duration = 300.0f;	// ms

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	App();
	~App();

	App(const App& x) = delete;
	App(App&& x) = delete;
	App& operator=(const App& rhs) = delete;

	/* MAIN FUNCTION =========================================================== */

	void								run();

	/* ========================================================================= */

	void							toggleTexture() noexcept;
	void							reset() noexcept;
	void							toggleMove(ObjectDirection direction) noexcept;
	void							untoggleMove(
		ObjectDirection direction
	) noexcept;
	void							updateCameraDir(
		float x,
		float y
	) noexcept;
	void							toggleLightColor() noexcept;
	void							toggleLightPos() noexcept;

private:
	/* ========================================================================= */
	/*                                  TYPEDEF                                  */
	/* ========================================================================= */

	typedef	std::chrono::high_resolution_clock::time_point	time_point;

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

	TextureState					texture_state = TextureState::TEXTURE_ENABLED;
	std::optional<time_point>		texture_transition_start;

	bool							keys_pressed_directions[6] = { false };
	scop::Vect3						movement{};
	std::size_t						selected_light_color = 0;
	std::size_t						selected_light_pos = 0;

	/* ========================================================================= */
	/*                               CONST MEMBERS                               */
	/* ========================================================================= */

	const scop::Vect3				light_colors[4] = {
		scop::Vect3(1.0f, 1.0f, 1.0f), // white
		scop::Vect3(1.0f, 0.0f, 0.0f), // red
		scop::Vect3(0.0f, 1.0f, 0.0f), // green
		scop::Vect3(0.0f, 0.0f, 1.0f) // blue
	};
	const scop::Vect3				light_positions[4] = {
		scop::Vect3(1.0f, 1.5f, 2.0f),
		scop::Vect3(0.0f, 0.5f, 0.5f),
		scop::Vect3(-1.0f, -1.8f, 1.75f),
		scop::Vect3(0.0f, -1.0f, 0.0f)
	};

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void								drawFrame();
	void								loadModel(const std::string& path);
	void								loadTerrain();
	void								update();

}; // class App

} // namespace scop