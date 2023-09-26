/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 11:46:11 by etran             #+#    #+#             */
/*   Updated: 2023/09/25 14:50:03 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chunk.h"
#include "perlin_noise.h"
#include "vertex.h"
#include "packed_cube.h"

namespace vox {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

Chunk::Chunk(const PerlinNoise& noise, uint8_t x, uint8_t y, uint8_t z):
	_x(x), _y(y), _z(z) {
		// _blocks.reserve(CHUNK_VOLUME);
		_generateChunk(noise);
}

/* ========================================================================== */

/**
 * @brief Generates a flat surface terrain mesh (z = 0) of size CHUNK_SIZE.
 * The chunk position is set to {0, 0, 0}.
*/
Chunk::ChunkMesh	Chunk::generateChunkMesh() noexcept {
	ChunkMesh	mesh;

	mesh.vertices.reserve(CHUNK_VERTICES_COUNT);
	mesh.indices.reserve(CHUNK_INDICES_COUNT);

	auto	addFace =
		[&mesh](const PackedCube::Face& face) -> void {
			// Indices
			uint32_t pos = mesh.vertices.size();
			uint32_t e = pos;
			uint32_t f = pos + 1;
			uint32_t g = pos + 2;
			uint32_t h = pos + 3;

			mesh.indices.emplace_back(e);
			mesh.indices.emplace_back(g);
			mesh.indices.emplace_back(f);

			mesh.indices.emplace_back(e);
			mesh.indices.emplace_back(h);
			mesh.indices.emplace_back(g);

			for (auto& uvertex: face.vertices) {
				// Vertices
				mesh.vertices.emplace_back(Vertex(uvertex.x, uvertex.y, uvertex.z));
			}
		};

	for (uint8_t z = 0; z < CHUNK_SIZE; ++z) {
		for (uint8_t x = 0; x < CHUNK_SIZE; ++x) {
			PackedCube	cube(x, z);

			addFace(cube.top());
			addFace(cube.bottom());
			addFace(cube.left());
			addFace(cube.right());
			addFace(cube.front());
			addFace(cube.back());
		}
	}

	return mesh;
}

// std::array<uint8_t, CHUNK_AREA>	Chunk::getHeightMap() const noexcept {
const std::array<uint8_t, CHUNK_AREA>&	Chunk::getHeightMap() const noexcept {
	// std::array<uint8_t, CHUNK_AREA>	height_map{};

	// for (uint8_t z = 0; z < CHUNK_SIZE; ++z) {
	// 	for (uint8_t x = 0; x < CHUNK_SIZE; ++x) {

	// 		uint8_t height = 0;
	// 		for (uint8_t y = 0; y < CHUNK_SIZE; ++y) {
	// 			// const Block& block = _blocks[z * CHUNK_SIZE + (y * CHUNK_SIZE + x)];
	// 			// if (block.getType() != MaterialType::MATERIAL_AIR) {
	// 			// 	height_map[z * CHUNK_SIZE + x] = y;
	// 			// 	break;
	// 			// }

	// 		}

	// 	}
	// }
	// return height_map;
	return _blocks;
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

/**
 * @brief Fills chunk volume data with blocks.
*/
void	Chunk::_generateChunk(const PerlinNoise& perlin_noise) {
	// _blocks.resize(CHUNK_VOLUME);

	for (uint8_t z = 0; z < CHUNK_SIZE; ++z) {
		for (uint8_t x = 0; x < CHUNK_SIZE; ++x) {
			float y = perlin_noise.noiseAt(_x * CHUNK_SIZE + x, _z * CHUNK_SIZE + z);
			_blocks[z * CHUNK_SIZE + x] = static_cast<uint8_t>(y);

			// Block& block = _blocks[z * CHUNK_SIZE + (y * CHUNK_SIZE + x)];

			//uint8_t height = static_cast<int32_t>((y / 16) & 0xFF);
			// block.setType(MaterialType::MATERIAL_GRASS);
			// TODO
			// _fillColumn(x, y);
		}
	}
}

} // namespace vox
