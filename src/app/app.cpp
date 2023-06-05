/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 11:12:12 by eli               #+#    #+#             */
/*   Updated: 2023/06/06 00:46:21 by etran            ###   ########.fr       */
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
	reset();
	window.init(this);
	engine.init(window, *image, light, vertices, indices);
}

App::~App() {
	engine.destroy();
}

/* ========================================================================== */

void	App::run() {
	while (window.alive()) {
		window.poll();
		update();
		drawFrame();
	}
	engine.idle();
}

/* ========================================================================== */

/**
 * On toggle, changes the texture of the model.
*/
void	App::toggleTexture() noexcept {
	texture_state = static_cast<TextureState>(
		(static_cast<int>(texture_state) + 1) % 3
	);
	texture_transition_start.emplace(
		std::chrono::high_resolution_clock::now()
	);
}

/**
 * Resets the model to its original position and rotation.
*/
void	App::reset() noexcept {
	// Reset player
	game.player.reset(game.world_origin, {0.0f, 0.0f, 1.0f});
}

/**
 * On toggle, the model is moved in the given direction.
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
 * On untoggle, the model stops moving in the given direction.
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

void	App::toggleLightColor() noexcept {
	selected_light_color = (selected_light_color + 1) % 4;
}

void	App::toggleLightPos() noexcept {
	selected_light_pos = (selected_light_pos + 1) % 4;
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

void	App::drawFrame() {
	engine.render(window, game.player);
}

void	App::update() {
	game.player.move(movement);
}

/* ========================================================================== */

void	App::loadTerrain() {
	LOG("Loading terrain...");
	const constexpr std::size_t	chunk_size = 16;
	const constexpr std::size_t	render_distance_xy = 4;
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
	for (const auto& coord: mesh.vertices) {
		scop::Vertex	vertex{};

		vertex.pos = coord;
		vertex.tex_coord = {0.0f, 0.0f}; // TODO
		vertex.normal = {0.0f, 1.0f, 0.0f}; // TODO
		math::generateVibrantColor(
			vertex.color.x,
			vertex.color.y,
			vertex.color.z
		);
		vertices.emplace_back(vertex);
	}
	indices = std::move(mesh.indices);
	LOG("Terrain loaded.");

	game.setOrigin(mesh.origin);

	scop::PpmLoader	img_loader(SCOP_TEXTURE_FILE_DEFAULT);
	image.reset(new scop::Image(img_loader.load()));
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
			vertex.tex_coord = {
				model_textures[index.texture].x,
				1.0f - model_textures[index.texture].y
			};
			vertex.normal = model_normals[index.normal];
			math::generateVibrantColor(
				vertex.color.x,
				vertex.color.y,
				vertex.color.z
			);

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
	image.reset(
		new scop::Image(std::move(*model.getMaterial().ambient_texture))
	);
	model.getMaterial().ambient_texture.release();

	// Load light
	light = UniformBufferObject::Light{
		model.getMaterial().ambient_color,
		light_positions[0],
		light_colors[0],
		model.getMaterial().diffuse_color,
		game.player.getPosition(),
		model.getMaterial().specular_color,
		model.getMaterial().shininess
	};
}

} // namespace scop