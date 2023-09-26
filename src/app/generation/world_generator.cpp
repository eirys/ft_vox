/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world_generator.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 12:06:07 by etran             #+#    #+#             */
/*   Updated: 2023/09/22 10:40:45 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "world_generator.h"
#include "world.h"

namespace vox {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

WorldGenerator::WorldGenerator(std::size_t seed):
	_noise(_getDefaultMapInfo(seed)) {}

/* ========================================================================== */

World		WorldGenerator::generate() const noexcept {
	return World(_noise, _width, _height);
}


// PerlinNoise::PerlinMesh	WorldGenerator::toPerlinMesh() const {
// 	PerlinNoise::PerlinMesh	mesh;

// 	auto	addFace =
// 		[&mesh]
// 		(const Cube::Face& face) -> void {
// 			for (std::size_t i = 0; i < 4; ++i) {
// 				uint8_t	face_index =
// 					face.side == FaceType::FACE_TOP ? 0 : (
// 					face.side == FaceType::FACE_BOTTOM ? 2 : 1);
// 				vox::Vertex	vertex(
// 					face.vertices[i],
// 					static_cast<uint8_t>(face.side),
// 					i,
// 					face_index);
// 				mesh.vertices.emplace_back(vertex);
// 			}
// 		};

// 	auto	addIndices =
// 		[&mesh]
// 		(uint32_t pos) -> void {
// 			uint32_t e = pos;
// 			uint32_t f = pos + 1;
// 			uint32_t g = pos + 2;
// 			uint32_t h = pos + 3;

// 			// First triangle
// 			mesh.indices.emplace_back(e);
// 			mesh.indices.emplace_back(g);
// 			mesh.indices.emplace_back(f);

// 			// Second triangle
// 			mesh.indices.emplace_back(e);
// 			mesh.indices.emplace_back(h);
// 			mesh.indices.emplace_back(g);
// 		};

// 	// TODO: Use a better algorithm
// 	// TODO store map

// 	for (std::size_t row = 0; row < _height; ++row) {
// 		for (std::size_t col = 0; col < _width; ++col) {
// 			// A pixel of the noise map will be a block.
// 			//   h_____g    Frame of reference:
// 			// e/|___f/|    y z
// 			// | d---|-c    |/
// 			// |/    |/     O___x
// 			// a_____b

// 			// Evaluate the current cube
// 			const float	perlin = _noise.noiseAt(col, row);

// 			const Cube	cube({
// 				static_cast<float>(col),
// 				perlin,
// 				static_cast<float>(row)
// 			});

// 			// Add the top face
// 			if (row != _height - 1 && col != _width - 1)
// 				addIndices(mesh.vertices.size());
// 			addFace(cube.top());

// 			// Check Perlin noise on side blocks
// 			// Add the south face
// 			if (row != 0) {
// 				const float	south = _noise.noiseAt(col, row - 1);
// 				if (south < perlin) {
// 					if (row != _height - 1 && col != _width - 1)
// 						addIndices(mesh.vertices.size());
// 					addFace(cube.back());
// 				}
// 			}
// 			// Add the north face
// 			if (row != _height - 1) {
// 				const float	north = _noise.noiseAt(col, row + 1);
// 				if (north < perlin) {
// 					if (col != _width - 1)
// 						addIndices(mesh.vertices.size());
// 					addFace(cube.front());
// 				}
// 			}
// 			// Add the west face
// 			if (col != 0) {
// 				const float	west = _noise.noiseAt(col - 1, row);
// 				if (west < perlin) {
// 					if (row != _height - 1 && col != _width - 1)
// 						addIndices(mesh.vertices.size());
// 					addFace(cube.left());
// 				}
// 			}
// 			// Add the east face
// 			if (col != _width - 1) {
// 				const float	east = _noise.noiseAt(col + 1, row);
// 				if (east < perlin) {
// 					if (row != _height - 1)
// 						addIndices(mesh.vertices.size());
// 					addFace(cube.right());
// 				}
// 			}
// 		}
// 	}

// 	return mesh;
// }

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

PerlinNoise::NoiseMapInfo	WorldGenerator::_getDefaultMapInfo(
	std::size_t seed
) const noexcept {
	PerlinNoise::NoiseMapInfo	noise_info{};
	noise_info.type = vox::PerlinNoiseType::PERLIN_NOISE_2D;
	noise_info.seed = seed;

	// World sizes
	noise_info.width = _width;
	noise_info.height = _height;
	noise_info.depth = _depth;

	// Noise generation info
	noise_info.layers = 3;
	noise_info.frequency_0 = .03f;
	noise_info.frequency_mult = 1.8f;
	noise_info.amplitude_mult = 0.5f;
	return noise_info;
}

} // namespace vox