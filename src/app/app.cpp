/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 11:12:12 by eli               #+#    #+#             */
/*   Updated: 2023/09/15 16:12:36 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app.h"
#include "scop_math.h"
#include "obj_parser.h"
#include "perlin_noise.h"
#include "utils.h"

#include "world_generator.h"
#include "world.h"

namespace scop {

/* ========================================================================== */
/*                                    OTHER                                   */
/* ========================================================================== */

static ObjectDirection	opposite(ObjectDirection dir) noexcept {
	if (dir == ObjectDirection::MOVE_FORWARD)
		return (ObjectDirection::MOVE_BACKWARD);
	else if (dir == ObjectDirection::MOVE_BACKWARD)
		return (ObjectDirection::MOVE_FORWARD);
	else if (dir == ObjectDirection::MOVE_RIGHT)
		return (ObjectDirection::MOVE_LEFT);
	else if (dir == ObjectDirection::MOVE_LEFT)
		return (ObjectDirection::MOVE_RIGHT);
	else if (dir == ObjectDirection::MOVE_UP)
		return (ObjectDirection::MOVE_DOWN);
	else
		return (ObjectDirection::MOVE_UP);
}

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

App::App() {
	_loadTerrain();
	resetGame();
	_window.init(this);
	_engine.init(_window, _game, _vertices, _indices);
}

App::~App() {
	_engine.destroy();
}

/* MAIN FUNCTION ============================================================ */

void	App::run() {
	_timer.start();
	while (_window.alive()) {
		_window.poll();
		_updateGame();
		_engine.render(_window, _game, _timer);
		_timer.check();
	}
	_engine.idle();
}

/* ========================================================================== */

/**
 * @brief Resets state of the game.
*/
void	App::resetGame() noexcept {
	_game.reset();
}

/**
 * @brief On toggle, the model is moved in the given direction.
*/
void	App::toggleMove(ObjectDirection dir) noexcept {
	_keys_pressed_directions[dir] = true;
	switch (dir) {
		case ObjectDirection::MOVE_FORWARD: {
			_movement.z = SCOP_MOVE_SPEED;
			break;
		}
		case ObjectDirection::MOVE_BACKWARD: {
			_movement.z = -SCOP_MOVE_SPEED;
			break;
		}
		case ObjectDirection::MOVE_RIGHT: {
			_movement.x = SCOP_MOVE_SPEED;
			break;
		}
		case ObjectDirection::MOVE_LEFT: {
			_movement.x = -SCOP_MOVE_SPEED;
			break;
		}
		case ObjectDirection::MOVE_UP: {
			_movement.y = SCOP_MOVE_SPEED;
			break;
		}
		case ObjectDirection::MOVE_DOWN: {
			_movement.y = -SCOP_MOVE_SPEED;
			break;
		}
		default:
			return;
	}
}

/**
 * @brief On untoggle, the model stops moving in the given direction.
*/
void	App::untoggleMove(ObjectDirection dir) noexcept {
	_keys_pressed_directions[dir] = false;
	if (_keys_pressed_directions[opposite(dir)] == false) {
		if (
			dir == ObjectDirection::MOVE_FORWARD ||
			dir == ObjectDirection::MOVE_BACKWARD
		) {
			_movement.z = 0.0f;
		} else if (
			dir == ObjectDirection::MOVE_RIGHT ||
			dir == ObjectDirection::MOVE_LEFT
		) {
			_movement.x = 0.0f;
		} else if (
			dir == ObjectDirection::MOVE_UP ||
			dir == ObjectDirection::MOVE_DOWN
		) {
			_movement.y = 0.0f;
		}
	} else {
		toggleMove(opposite(dir));
	}
}

void	App::updateCameraDir(float x, float y) noexcept {
	_game.setCameraDir(x, y);
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

/**
 * @brief Updates state of the game.
*/
void	App::_updateGame() {
	_game.setCameraPos(_movement);
}

void	App::_loadTerrain() {
	using namespace vox;
	LOG("Loading terrain...");

	WorldGenerator	generator(42);
	World			world = generator.generate();
	vox::PerlinNoise::PerlinMesh	mesh = generator.toPerlinMesh();

	_vertices = std::move(mesh.vertices);
	_indices = std::move(mesh.indices);
	LOG("Terrain loaded.");
}

} // namespace scop