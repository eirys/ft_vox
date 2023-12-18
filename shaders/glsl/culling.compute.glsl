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
OUTPUT(CULLING_SET, 2, std140)	buffer QuadCount {
	uint n[MAX_RENDER_DISTANCE];
}	quadCount;

OUTPUT(CULLING_SET, 3, std140)	buffer SampleData {
	uint info[MAX_RENDER_DISTANCE][CHUNK_VOLUME];	// x: 4 bits, y: 4 bits, z: 4 bits, layer: 12 bits (or 16 bits? TODO)
} sampleData;

OUTPUT(CULLING_SET, 4, std140)	buffer VerticesData {
	// TODO Can be compressed to 16 bits ?
	// Per chunk / per block / per face
	uint vertexData[MAX_RENDER_DISTANCE][CHUNK_VOLUME][6]; // blockId: 12 bit, face: 4 bits, unused: 16 bits
} verticesData;

/* UNIFORMS ================================================================= */
UNIFORM(CULLING_SET, 0)		Frustum { vec4 planes[6]; }		frustum;
UNIFORM(CULLING_SET, 1)		usampler2DArray					chunks; // r: type, g: properties

/* ========================================================================== */
/*                                  INCLUDES                                  */
/* ========================================================================== */

#include "scene_rendering_tools.glslh"

/* ========================================================================== */

struct AABB {
	vec3	center;
	vec3	extent;
};

AABB	getAABB() {
	AABB box;

	float halfChunkSize = float(CHUNK_SIZE) / 2.0f;

	box.extent = vec3(halfChunkSize);
	box.center =	vec3((float(gl_GlobalInvocationID.x)),
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

ivec3	getBlockOffset(in uint _side) {
	switch (_side) {
		case RIGHT:		return ivec3(1, 0, 0);
		case LEFT:		return ivec3(-1, 0, 0);
		case TOP:		return ivec3(0, 1, 0);
		case BOTTOM:	return ivec3(0, -1, 0);
		case FRONT:		return ivec3(0, 0, 1);
		case BACK:		return ivec3(0, 0, -1);
	}
	return ivec3(0);
}

// TODO
// void	updateSampleData(inout ivec3 _blockPos, inout _chunkId) {
// 	uint blockPos = (_blockPos.x << 28) | (_blockPos.y << 24) | (_blockPos.z << 20);
// 	sampleData.info[blockPos] = (_blockId << 20) | _blockLayer;
// }

void	checkNeighborBlock(inout ivec3 _neighborPos, inout uint _neighborChunkId) {
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

bool	isAABBOutside(in vec4 _plane, in AABB _box) {
	float r = dot(_box.extent, abs(_plane.xyz));
	float d = dot(_box.center, _plane.xyz) + _plane.w;
	return (d + r) < 0;
}

// Frustum culling
bool	isChunkVisible(in AABB _box) {
	for (uint i = 0; i < 6; ++i) {
		if (isAABBOutside(frustum.planes[i], _box))
			return false;
	}
	return true;
}

// Face culling
bool	isFaceVisible(in uvec3 _blockPos, in uint _chunkId, in uint _face) {
	ivec3 neighborBlockPos = ivec3(_blockPos) + getBlockOffset(_face);
	uint  neighborBlockChunk = _chunkId;
	checkNeighborBlock(neighborBlockPos, neighborBlockChunk);

	uvec2 neighborBlock = getBlock(uvec3(neighborBlockPos), neighborBlockChunk);
	if (neighborBlock.r == EMPTY_BLOCK || (neighborBlock.g & TRANSPARENT_MASK))
		return true;
	return false;
}

/* ========================================================================== */

void	checkBlockFaces(in uvec3 _blockPos, in uint _chunkId, in uint _blockProperties) {
	if (_blockType == 0)
		return;

	// TODO: Semi-transparent blocs

	// if (_blockType & ALPHACLIPPED_MASK) {
	//	Do sth
	// }

	for (uint face = 0; face < 6; ++face) {

		if (isFaceVisible(_blockPos, face)) {
			++quadCount.n[_chunkId];

			uint blockId = _blockPos.x << 12 | _blockPos.y << 8 | _blockPos.z << 4 | face;

			uint blockIndex = z * CHUNK_AREA + (y * CHUNK_SIZE + x);
			verticesData.vertexData[_chunkId][blockIndex][face] = blockId;
		}

	}
}

/* ENTRYPOINT =============================================================== */
void main() {
	if (!isChunkVisible(getAABB()))
		return;

	uint chunkId = (gl_GlobalInvocationID.x << 12)
				 + (gl_GlobalInvocationID.y << 8)
				 + (gl_GlobalInvocationID.z << 4);

	for (uint z = 0; z < CHUNK_SIZE; ++z) {
		for (uint y = 0; y < CHUNK_SIZE; ++y) {
			for (uint x = 0; x < CHUNK_SIZE; ++x) {
				uvec3 blockPos = uvec3(x, y, z);
				uvec2 blockData = getBlock(blockPos, chunkId);

				checkBlockFaces(blockPos, chunkId, blockData.g);
			}

		}
	}
}
