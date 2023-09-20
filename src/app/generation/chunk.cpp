/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 11:46:11 by etran             #+#    #+#             */
/*   Updated: 2023/09/19 11:46:11 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chunk.h"
#include "perlin_noise.h"
// #include "vertex.h"

// using scop::Vertex;

struct Vertex {

	int32_t	packed_position; // cf. toChunkPos
	int32_t packed_data; // cf. face index,
	// TODO

};

namespace vox {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

Chunk::Chunk(const PerlinNoise& noise, uint8_t x, uint8_t y, uint8_t z):
	_x(x), _y(y), _z(z) {
		_generateChunk(noise);
}

/**
 * @brief Generates a flat surface terrain mesh (z = 0) of size CHUNK_SIZE.
 * The chunk position is set to {0, 0, 0}.
*/
Chunk::ChunkMesh	Chunk::generateChunkMesh() noexcept {
	ChunkMesh	mesh;

	auto	addFace =
		[&mesh]
		(const Cube::Face& face) -> void {
			for (std::size_t i = 0; i < 4; ++i) {
				uint8_t	face_index =
					face.side == FaceType::FACE_TOP ? 0 : (
					face.side == FaceType::FACE_BOTTOM ? 2 : 1);
				Vertex	vertex(
					face.vertices[i],
					static_cast<uint8_t>(face.side),
					i,
					face_index);
				*mesh.vertices.end() = vertex;
			}
		};

	auto	addIndices =
		[&mesh]
		(uint32_t pos) -> void {
			uint32_t e = pos;
			uint32_t f = pos + 1;
			uint32_t g = pos + 2;
			uint32_t h = pos + 3;

			// First triangle
			*mesh.indices.end() = e;
			*mesh.indices.end() = g;
			*mesh.indices.end() = f;

			// Second triangle
			*mesh.indices.end() = e;
			*mesh.indices.end() = h;
			*mesh.indices.end() = g;
		};

	constexpr std::size_t	terrain_size = CHUNK_SIZE;
	for (std::size_t y = 0; y < terrain_size; ++y) {
		for (std::size_t x = 0; x < terrain_size; ++x) {
			Cube	cube({x, y, 0.0f});


		}
	}
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

// Fill surface, then fill column
void	Chunk::_generateChunk(const PerlinNoise& perlin_noise) {
	for (uint8_t y = 0; y < CHUNK_SIZE; ++y) {
		for (uint8_t x = 0; x < CHUNK_SIZE; ++x) {
			float z = perlin_noise.noiseAt(x, y);
			_blocks[z * CHUNK_SIZE + (y * CHUNK_SIZE + x)].setType(MaterialType::MATERIAL_GRASS);
			// _fillColumn(x, y);
		}
	}
}

} // namespace vox
