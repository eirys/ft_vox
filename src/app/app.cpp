/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 11:12:12 by eli               #+#    #+#             */
/*   Updated: 2023/06/29 16:00:04 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app.h"
#include "model.h"
#include "obj_parser.h"
#include "ppm_loader.h"
#include "math.h"
#include "mtl_parser.h"
#include "perlin_noise.h"

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
	loadTerrain();
	resetGame();
	scop::mtl::Material material;
	material.ambient_color = {0.5,0.5,0.5};
	loadLight(material);
	window.init(this);
	engine.init(window, textures, light, vertices, indices);
}

App::~App() {
	engine.destroy();
}

/* MAIN FUNCTION ============================================================ */

void	App::run() {
	timer.start();
	while (window.alive()) {
		window.poll();
		updateGame();
		engine.render(window, game.player, timer);
		timer.check();
	}
	engine.idle();
}

/* ========================================================================== */

/**
 * @brief Resets the model to its original position and rotation.
*/
void	App::resetGame() noexcept {
	// Reset player
	game.player.reset(game.world_origin, {0.0f, 0.0f, 1.0f});
}

/**
 * @brief On toggle, the model is moved in the given direction.
*/
void	App::toggleMove(ObjectDirection dir) noexcept {
	keys_pressed_directions[dir] = true;
	switch (dir) {
		case ObjectDirection::MOVE_FORWARD: {
			movement.z = SCOP_MOVE_SPEED;
			break;
		}
		case ObjectDirection::MOVE_BACKWARD: {
			movement.z = -SCOP_MOVE_SPEED;
			break;
		}
		case ObjectDirection::MOVE_RIGHT: {
			movement.x = SCOP_MOVE_SPEED;
			break;
		}
		case ObjectDirection::MOVE_LEFT: {
			movement.x = -SCOP_MOVE_SPEED;
			break;
		}
		case ObjectDirection::MOVE_UP: {
			movement.y = SCOP_MOVE_SPEED;
			break;
		}
		case ObjectDirection::MOVE_DOWN: {
			movement.y = -SCOP_MOVE_SPEED;
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
	keys_pressed_directions[dir] = false;
	if (keys_pressed_directions[opposite(dir)] == false) {
		if (
			dir == ObjectDirection::MOVE_FORWARD ||
			dir == ObjectDirection::MOVE_BACKWARD
		) {
			movement.z = 0.0f;
		} else if (
			dir == ObjectDirection::MOVE_RIGHT ||
			dir == ObjectDirection::MOVE_LEFT
		) {
			movement.x = 0.0f;
		} else if (
			dir == ObjectDirection::MOVE_UP ||
			dir == ObjectDirection::MOVE_DOWN
		) {
			movement.y = 0.0f;
		}
	} else {
		toggleMove(opposite(dir));
	}
}

void	App::updateCameraDir(float x, float y) noexcept {
	game.player.updateEyeDir(x, y);
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

/**
 * @brief Updates state of the game.
*/
void	App::updateGame() {
	game.player.move(movement);
}

/* INIT FUNCTIONS =========================================================== */

void	App::loadTerrain() {
	LOG("Loading terrain...");
	const constexpr std::size_t	chunk_size = 16;
	const constexpr std::size_t	render_distance_xy = 10;
	const constexpr std::size_t	render_distance_z = 2;

	vox::PerlinNoise	noise({
		.type = vox::PerlinNoiseType::PERLIN_NOISE_2D,
		.seed = 42,
		.width = render_distance_xy * chunk_size,
		.height = render_distance_xy * chunk_size,
		.depth = render_distance_z * chunk_size,
		.layers = 4,
		.frequency_0 = .02f,
		.frequency_mult = 1.8f,
		.amplitude_mult = 0.5f
	});

	vox::PerlinNoise::PerlinMesh	mesh = noise.toMesh();
	vertices.reserve(mesh.vertices.size());
	for (std::size_t i = 0; i < mesh.vertices.size(); ++i) {
		scop::Vertex	vertex{};

		vertex.pos = mesh.vertices[i];
		vertex.uv = mesh.uvs[i];
		vertex.normal = mesh.normals[i];
		vertex.texture_id = mesh.texture_indices[i];

		vertices.emplace_back(vertex);
	}
	indices = std::move(mesh.indices);
	LOG("Terrain loaded.");

	game.setOrigin(mesh.origin);

	// TEMPORARY ===
	LOG("Loading textures...");
	const std::vector<std::string>	paths {
		SCOP_TEXTURE_PATH "grass_top.ppm",
		SCOP_TEXTURE_PATH "grass_side.ppm",
		SCOP_TEXTURE_PATH "dirt.ppm"
	};

	if (paths.size() > TEXTURE_SAMPLER_COUNT) {
		throw std::runtime_error("Too many textures to be loaded");
	}

	std::vector<scop::Image>	texture_elements(paths.size());
	for (std::size_t i = 0; i < paths.size(); ++i) {
		scop::PpmLoader	loader(paths[i]);
		texture_elements[i] = loader.load();
	}

	CubeMap	grass_cube_map {
		texture_elements[1],
		texture_elements[1],
		texture_elements[1],
		texture_elements[1],
		std::move(texture_elements[0]),
		std::move(texture_elements[2])
	};

	textures.reserve(1);
	textures.emplace_back(std::move(grass_cube_map));
	LOG("Textures loaded.");
}

void	App::loadModel(const std::string& path) {
	LOG("Loading model...");

	scop::obj::ObjParser	parser;
	scop::obj::Model	model = parser.parseFile(path.c_str());

	std::unordered_map<scop::Vertex, uint32_t>	unique_vertices{};

	const auto&	model_vertices = model.getVertexCoords();
	const auto& model_textures = model.getTextureCoords();
	const auto& model_normals = model.getNormalCoords();
	const auto& model_triangles = model.getTriangles();

	// Retrieve unique vertices:
	for (const auto& triangle: model_triangles) {
		for (const auto& index: triangle.indices) {
			scop::Vertex	vertex{};

			vertex.pos = model_vertices[index.vertex];
			vertex.uv = {
				model_textures[index.texture].x,
				1.0f - model_textures[index.texture].y
			};
			vertex.normal = model_normals[index.normal];
			// math::generateVibrantColor(
			// 	vertex.color.x,
			// 	vertex.color.y,
			// 	vertex.color.z
			// );

			if (unique_vertices.count(vertex) == 0) {
				unique_vertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.emplace_back(vertex);
			}
			indices.emplace_back(unique_vertices[vertex]);
		}
	}

	// Center model
	scop::Vect3	barycenter = utils::computeBarycenter(vertices);
	for (auto& vertex: vertices) {
		vertex.pos -= barycenter;
	}

	// Pass ownership of texture image from model to app
	//TODO
	// textures.emplace_back(
	// 	std::move(*model.getMaterial().ambient_texture)
	// );
	// model.getMaterial().ambient_texture.release();
}

void	App::loadLight(const scop::mtl::Material& mat) {
	light = scop::UniformBufferObject::Light{
		.ambient_color = mat.ambient_color,
		.light_vector = scop::normalize(scop::Vect3(0.1f, 1.0f, 0.3f)),
		.light_color = scop::Vect3(1.0f, 1.0f, 0.8f),
		.light_intensity = 0.4f
	};
}

} // namespace scop