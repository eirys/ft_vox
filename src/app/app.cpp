/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 11:12:12 by eli               #+#    #+#             */
/*   Updated: 2023/06/04 22:22:24 by etran            ###   ########.fr       */
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

TextureState					App::texture_state = TextureState::TEXTURE_ENABLED;
std::optional<App::time_point>	App::texture_transition_start;

std::map<RotationInput, bool>	App::keys_pressed_rotations = populateRotationKeys();
scop::Vect3						App::rotation_angles = scop::Vect3(0.0f, 0.0f, 0.0f);
scop::Vect3						App::rotating_input = scop::Vect3(0.0f, 0.0f, 0.0f);

std::map<ObjectDirection, bool>	App::keys_pressed_directions = populateDirectionKeys();
scop::Vect3						App::movement = scop::Vect3(0.0f, 0.0f, 0.0f);
scop::Vect3						App::position = scop::Vect3(1.0f, 10.0f, 3.0f);
scop::Vect3						App::eye_dir = scop::normalize(-App::position);

std::array<scop::Vect3, 4>		App::light_colors = {
	scop::Vect3(1.0f, 1.0f, 1.0f), // white
	scop::Vect3(1.0f, 0.0f, 0.0f), // red
	scop::Vect3(0.0f, 1.0f, 0.0f), // green
	scop::Vect3(0.0f, 0.0f, 1.0f) // blue
};
std::size_t						App::selected_light_color = 0;
std::array<scop::Vect3, 4>		App::light_positions = {
	scop::Vect3(1.0f, 1.5f, 2.0f),
	scop::Vect3(0.0f, 0.5f, 0.5f),
	scop::Vect3(-1.0f, -1.8f, 1.75f),
	scop::Vect3(0.0f, -1.0f, 0.0f)
};
std::size_t						App::selected_light_pos = 0;

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

App::App() {
	// loadModel(model_file);
	loadTerrain();
	window.init();
	engine.init(window, *image, light, vertices, indices);
}

App::~App() {
	engine.destroy();
}

/* ========================================================================== */

void	App::run() {
	while (window.alive()) {
		window.await();
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
void	App::resetModel() noexcept {
	// Reset rotation
	rotation_angles.x = 0.0f;
	rotation_angles.y = 0.0f;
	rotation_angles.z = 0.0f;

	// Reset translation
	position = scop::Vect3(1.0f, 1.0f, 3.0f);
}

/**
 * On toggle, changes the rotation of the model.
*/
void	App::toggleRotation(RotationInput value) noexcept {
	keys_pressed_rotations[value] = true;
	switch (value) {
		case RotationInput::ROTATION_ADD_X: {
			rotating_input.x = SCOP_ROTATION_SPEED;
			break;
		}
		case RotationInput::ROTATION_SUB_X: {
			rotating_input.x = -SCOP_ROTATION_SPEED;
			break;
		}
		case RotationInput::ROTATION_ADD_Y: {
			rotating_input.y = SCOP_ROTATION_SPEED;
			break;
		}
		case RotationInput::ROTATION_SUB_Y: {
			rotating_input.y = -SCOP_ROTATION_SPEED;
			break;
		}
		case RotationInput::ROTATION_ADD_Z: {
			rotating_input.z = SCOP_ROTATION_SPEED;
			break;
		}
		case RotationInput::ROTATION_SUB_Z: {
			rotating_input.z = -SCOP_ROTATION_SPEED;
			break;
		}
		default:
			return;
	}
}

/**
 * On untoggle, stops the rotation of the model.
*/
void	App::untoggleRotation(RotationInput value) noexcept {
	keys_pressed_rotations[value] = false;
	if (keys_pressed_rotations[static_cast<RotationInput>(-value)] == false) {
		if (
			value == RotationInput::ROTATION_ADD_X ||
			value == RotationInput::ROTATION_SUB_X
		) {
			rotating_input.x = 0.0f;
		} else if (
			value == RotationInput::ROTATION_ADD_Y ||
			value == RotationInput::ROTATION_SUB_Y
		) {
			rotating_input.y = 0.0f;
		} else if (
			value == RotationInput::ROTATION_ADD_Z ||
			value == RotationInput::ROTATION_SUB_Z
		) {
			rotating_input.z = 0.0f;
		}
	} else {
		toggleRotation(static_cast<RotationInput>(-value));
	}
}

/**
 * On toggle, the model is moved in the given direction.
*/
void	App::toggleMove(ObjectDirection dir) noexcept {
	keys_pressed_directions[dir] = true;
	switch (dir) {
		case ObjectDirection::MOVE_FORWARD: {
			movement.z = -SCOP_MOVE_SPEED;
			break;
		}
		case ObjectDirection::MOVE_BACKWARD: {
			movement.z = SCOP_MOVE_SPEED;
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
	if (keys_pressed_directions[static_cast<ObjectDirection>(-dir)] == false) {
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
		toggleMove(static_cast<ObjectDirection>(-dir));
	}
}

void	App::updateCameraDir(float x, float y) noexcept {
	static float last_x = x;
	static float last_y = y;

	// Retrieve values from current eye_dir
	static float yaw = math::dregrees(std::atan2(eye_dir.z, eye_dir.x));
	static float pitch = math::dregrees(std::asin(eye_dir.y));

	yaw += (x - last_x) * SCOP_MOUSE_SENSITIVITY;
	pitch = std::clamp(
		std::fma(last_y - y, SCOP_MOUSE_SENSITIVITY, pitch),
		-89.f,
		89.f
	);

	last_x = x;
	last_y = y;

	eye_dir.x = std::cos(math::radians(yaw)) * std::cos(math::radians(pitch));
	eye_dir.y = std::sin(math::radians(pitch));
	eye_dir.z = std::sin(math::radians(yaw)) * std::cos(math::radians(pitch));
	eye_dir = scop::normalize(eye_dir);
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
	update();
	engine.render(window, indices.size());
}

void	App::update() {
	// Update camera position
	position += movement;

	// Update model rotation
	rotation_angles.x += rotating_input.x;
	rotation_angles.y += rotating_input.y;
	rotation_angles.z += rotating_input.z;
}

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
		vertex.tex_coord = {0.0f, 0.0f};
		vertex.normal = {0.0f, 1.0f, 0.0f};
		math::generateVibrantColor(
			vertex.color.x,
			vertex.color.y,
			vertex.color.z
		);
		vertices.emplace_back(vertex);
	}
	indices = std::move(mesh.indices);
	LOG("Terrain loaded.");

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
		App::light_positions[0],
		App::light_colors[0],
		model.getMaterial().diffuse_color,
		App::position,
		model.getMaterial().specular_color,
		model.getMaterial().shininess
	};
}

/* ========================================================================== */
/*                                    OTHER                                   */
/* ========================================================================== */

std::map<ObjectDirection, bool>	populateDirectionKeys() {
	std::map<ObjectDirection, bool>	map;

	map[ObjectDirection::MOVE_FORWARD] = false;
	map[ObjectDirection::MOVE_BACKWARD] = false;
	map[ObjectDirection::MOVE_LEFT] = false;
	map[ObjectDirection::MOVE_RIGHT] = false;
	map[ObjectDirection::MOVE_UP] = false;
	map[ObjectDirection::MOVE_DOWN] = false;
	return map;
}

std::map<RotationInput, bool> populateRotationKeys() {
	std::map<RotationInput, bool>	map;

	map[RotationInput::ROTATION_ADD_X] = false;
	map[RotationInput::ROTATION_SUB_X] = false;
	map[RotationInput::ROTATION_ADD_Y] = false;
	map[RotationInput::ROTATION_SUB_Y] = false;
	map[RotationInput::ROTATION_ADD_Z] = false;
	map[RotationInput::ROTATION_SUB_Z] = false;
	return map;
}

} // namespace scop