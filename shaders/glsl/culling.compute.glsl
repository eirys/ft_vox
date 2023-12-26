#version 450
#define CS

// Execute this compute for each chunk
// Each chunk is 16x16x16 blocs
// Compute shader will determine which blocs are visible or not and
// compute the number of vertices to draw + the vertices data

// Input:
//  - Frustum -> frustum culling
//  - Blocs data ('chunk') -> determine if a bloc face is visible or not
//     -> output: gives number of vertices to draw + vertices data
//     (position, normal, texture coordinates)

/* ========================================================================== */
/*                                   MACROS                                   */
/* ========================================================================== */

#include "decl.glslh"
#include "../../src/app/generation/chunk_macros.h"
#include "../../src/app/engine/gfx/renderers/culling/culling_consts.h"

/* ========================================================================== */
/*                                 SHADER I/O                                 */
/* ========================================================================== */

// Input
INPUT(16, 16, 16);

// Output
OUTPUT(CULLING_SET, 2, std140)	uniform writeonly buffer QuadCount {
	uint n[MAX_RENDER_DISTANCE];
}	quadCount;

OUTPUT(CULLING_SET, 3, std140)	uniform writeonly buffer VerticesData {
	// Per chunk / per block / per face
	uint vertexData[MAX_RENDER_DISTANCE][CHUNK_VOLUME][6];
	// blockId: 12 bit		-> to retrieve block type in `chunks` + block position
	// face: 4 bits			-> to retrieve which face to sample
	// layer: 12 bits		-> to retrieve which chunk to offset block position
	// orientation: 1 bit	-> swap uv orientation for directed faces
	// unused: 3 bits
} verticesData;

OUTPUT(CULLING_SET, 4, std140) uniform writeonly buffer Indexer {
	uint index[MAX_RENDER_DISTANCE * CHUNK_VOLUME * 6];
} indexer;

/* UNIFORMS ================================================================= */
UNIFORM(CULLING_SET, 0)		Frustum { vec4 planes[6]; }		frustum;
UNIFORM(CULLING_SET, 1)		usampler2DArray					chunks; // r: type, g: properties, b: todo (block orientation data etc)

/* ========================================================================== */
/*                                  INCLUDES                                  */
/* ========================================================================== */

#include "scene_rendering_tools.glslh"

/* ========================================================================== */

struct AABB {
	vec3	center;
	vec3	extent;
};

AABB	_getAABB() {
	float halfChunkSize = float(CHUNK_SIZE) / 2.0f;

	AABB box;
	box.extent = vec3(halfChunkSize);
	box.center = vec3((float(gl_GlobalInvocationID.x)),
					  (float(gl_GlobalInvocationID.y)),
					  (float(gl_GlobalInvocationID.z)))
				 + halfChunkSize
				 * float(CHUNK_SIZE);
	return box;
}

# define RIGHT	0
# define LEFT	1
# define TOP	2
# define BOTTOM	3
# define FRONT	4
# define BACK	5

ivec3	_getBlockOffset(in uint _side) {
	switch (_side) {
		case RIGHT:		return ivec3(1, 0, 0);
		case LEFT:		return ivec3(-1, 0, 0);
		case TOP:		return ivec3(0, 1, 0);
		case BOTTOM:	return ivec3(0, -1, 0);
		case FRONT:		return ivec3(0, 0, 1);
		case BACK:		return ivec3(0, 0, -1);
	}
	return ivec3(0); // shouldnt happen
}

// TODO
// void	updateSampleData(inout ivec3 _blockPos, inout _chunkId) {
// 	uint blockPos = (_blockPos.x << 28) | (_blockPos.y << 24) | (_blockPos.z << 20);
// 	sampleData.info[blockPos] = (_blockId << 20) | _blockLayer;
// }

void	_checkNeighborBlock(inout ivec3 _neighborPos, inout uint _neighborChunkId) {
	// TODO update neighbor pos

	// if (_blockPos.x < 0) {
	// 	_blockPos.x += CHUNK_SIZE;
	// 	_chunkId -= 1;
	// } else if (_blockPos.x >= CHUNK_SIZE) {
	// 	_blockPos.x -= CHUNK_SIZE;
	// 	_chunkId += 1;
	// }
}

/* ========================================================================== */

bool	_isAABBOutside(in vec4 _plane, in AABB _box) {
	float r = dot(_box.extent, abs(_plane.xyz));
	float d = dot(_box.center, _plane.xyz) + _plane.w;
	return (d + r) < 0;
}

// Frustum culling
bool	_isChunkVisible(in AABB _box) {
	for (uint i = 0; i < 6; ++i) {
		if (_isAABBOutside(frustum.planes[i], _box))
			return false;
	}
	return true;
}

// Face culling
bool	_isFaceVisible(in uvec3 _blockPos, in uint _chunkId, in uint _face) {
	uvec2 blockData = getBlock(_blockPos, _chunkId);
	if ((blockData.r == EMPTY_BLOCK) || bool(blockData.g & TRANSPARENT_MASK))
		return false;

	ivec3 neighborBlockPos = ivec3(_blockPos) + _getBlockOffset(_face);
	uint  neighborBlockChunk = _chunkId;
	_checkNeighborBlock(neighborBlockPos, neighborBlockChunk);

	uvec2 neighborBlockData = getBlock(uvec3(neighborBlockPos), neighborBlockChunk);
	if ((neighborBlockData.r == EMPTY_BLOCK) || bool(neighborBlockData.g & TRANSPARENT_MASK))
		return true;

	return false;
}

/* ========================================================================== */

void	_checkBlockFaces(in uvec3 _blockPos, in uint _chunkId) {
	// TODO: Semi-transparent blocs

	for (uint face = 0; face < 6; ++face) {
		uint blockIndex = _blockPos.z * CHUNK_AREA + (_blockPos.y * CHUNK_SIZE + _blockPos.x);

		if (_isFaceVisible(_blockPos, _chunkId, face)) {
			++quadCount.n[_chunkId];
			uint blockId = _blockPos.x << 12 | _blockPos.y << 8 | _blockPos.z << 4 | face;
			verticesData.vertexData[_chunkId][blockIndex][face] = blockId;
		} else {
			// TODO: set value mask for unused val
			verticesData.vertexData[_chunkId][blockIndex][face] = 1 << 31;
		}
	}
}

/* ENTRYPOINT =============================================================== */
void main() {
	if (!_isChunkVisible(_getAABB()))
		return;

	uint chunkId = (gl_GlobalInvocationID.x << 12)
				 + (gl_GlobalInvocationID.y << 8)
				 + (gl_GlobalInvocationID.z << 4);

	for (uint z = 0; z < CHUNK_SIZE; ++z) {
		for (uint y = 0; y < CHUNK_SIZE; ++y) {
			for (uint x = 0; x < CHUNK_SIZE; ++x) {
				uvec3 blockPos = uvec3(x, y, z);
				_checkBlockFaces(blockPos, chunkId);
			}
		}
	}
}
