/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:17:06 by etran             #+#    #+#             */
/*   Updated: 2023/06/04 17:17:23 by etran            ###   ########.fr       */
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

# define SCOP_MOUSE_SENSITIVITY	0.25f
# define SCOP_MOVE_SPEED		0.05f
# define SCOP_ROTATION_SPEED	0.25f // deg

namespace scop {

enum RotationAxis {
	ROTATION_AXIS_X = 0,
	ROTATION_AXIS_Y = 1,
	ROTATION_AXIS_Z = 2
};

enum RotationInput {
	ROTATION_ADD_X = 1,
	ROTATION_SUB_X = -1,
	ROTATION_ADD_Y = 2,
	ROTATION_SUB_Y = -2,
	ROTATION_ADD_Z = 3,
	ROTATION_SUB_Z = -3
};

enum ObjectDirection {
	MOVE_FORWARD = 1,
	MOVE_BACKWARD = -1,
	MOVE_LEFT = 2,
	MOVE_RIGHT = -2,
	MOVE_UP = 3,
	MOVE_DOWN = -3
};

enum TextureState {
	TEXTURE_GRAYSCALE = 1,
	TEXTURE_COLOR = 2,
	TEXTURE_ENABLED = 0
};

/**
 * Core engine.
*/
class App {
public:

	friend graphics::DescriptorSet;

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

	static void							toggleTexture() noexcept;
	static void							resetModel() noexcept;
	static void							toggleRotation(
		RotationInput value
	) noexcept;
	static void							untoggleRotation(
		RotationInput value
	) noexcept;
	static void							toggleMove(
		ObjectDirection direction
	) noexcept;
	static void							untoggleMove(
		ObjectDirection direction
	) noexcept;
	static void							updateCameraDir(
		float x,
		float y
	) noexcept;
	static void							toggleLightColor() noexcept;
	static void							toggleLightPos() noexcept;

private:
	/* ========================================================================= */
	/*                                  TYPEDEF                                  */
	/* ========================================================================= */

	typedef	std::chrono::high_resolution_clock::time_point	time_point;

	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	scop::Window						window;
	scop::graphics::Engine				engine;

	std::vector<scop::Vertex>			vertices;
	std::vector<uint32_t>				indices;
	std::unique_ptr<scop::Image>		image;
	UniformBufferObject::Light			light;

	/* ========================================================================= */
	/*                               STATIC MEMBERS                              */
	/* ========================================================================= */

	static TextureState					texture_state;
	static std::optional<time_point>	texture_transition_start;

	static
	std::map<RotationInput, bool>		keys_pressed_rotations;
	static scop::Vect3					rotation_angles;
	static scop::Vect3					rotating_input;

	static
	std::map<ObjectDirection, bool>		keys_pressed_directions;
	static scop::Vect3					movement;
	static scop::Vect3					position;
	static scop::Vect3					eye_dir;

	static std::array<scop::Vect3, 4>	light_colors;
	static std::size_t					selected_light_color;
	static std::array<scop::Vect3, 4>	light_positions;
	static std::size_t					selected_light_pos;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void								drawFrame();
	void								loadModel(const std::string& path);
	void								loadTerrain();
	void								update();

}; // class App

/* ========================================================================== */
/*                                    OTHER                                   */
/* ========================================================================== */

std::map<ObjectDirection, bool>			populateDirectionKeys();
std::map<RotationInput, bool>			populateRotationKeys();

} // namespace scop