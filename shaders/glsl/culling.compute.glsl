#version 450
#define CS

// Execute this compute for two chunks
// Each chunk is 16x16x16 blocs
// Compute shader will determine which blocs are visible or not and
// compute the number of vertices to draw + the vertices data -> NOT AYNMORE

// Input:
//  - Frustum -> frustum culling
//  - Blocs data ('chunk') -> determine if a bloc face is visible or not
//     -> output: gives number of vertices to draw + vertices data
//     (position, normal, texture coordinates)

#define QUAD_COUNT
#define QUAD_DATA
#define FRUSTUM
#define CHUNKS

/* ========================================================================== */
/*                                   MACROS                                   */
/* ========================================================================== */

#include "decl.glslh"
#include "descriptors.glslh"

/* ========================================================================== */
/*                                 SHADER I/O                                 */
/* ========================================================================== */

// Input
INPUT(DISPATCH_SIZE, DISPATCH_SIZE, DISPATCH_SIZE);

// Output
OUTPUT(CULLING_SET, 2)	buffer			QUAD_COUNT_DESCRIPTOR;
OUTPUT(CULLING_SET, 3)	buffer			QUAD_DATA_DESCRIPTOR;

/* UNIFORMS ================================================================= */
UNIFORM(CULLING_SET, 0)					FRUSTUM_DESCRIPTOR;
UNIFORM(CULLING_SET, 1) usampler2DArray	CHUNKS_DESCRIPTOR;

/* ========================================================================== */
/*                                  INCLUDES                                  */
/* ========================================================================== */

#include "tools.glslh"

/* ========================================================================== */

struct AABB {
	vec3	center;
	vec3	extent;
};

AABB	_getAABB(in uvec3 _chunkUvw) {
	float halfChunkSize = float(CHUNK_SIZE) / 2.0f;

	AABB box;
	box.extent = vec3(halfChunkSize);
	box.center = vec3(_chunkUvw)
				 + halfChunkSize
				 * float(CHUNK_SIZE);
	return box;
}

bool	_isAABBOutside(in vec4 _plane, in AABB _box) {
	float r = dot(_box.extent, abs(_plane.xyz));
	float d = dot(_box.center, _plane.xyz) + _plane.w;
	return (d + r) < 0;
}

// Frustum culling
bool	_isChunkVisible(in AABB _box) {
	for (uint i = 0; i < FACE_COUNT; ++i) {
		if (_isAABBOutside(frustum[i], _box))
			return false;
	}
	return true;
}

// Check all faces of a bloc.
void	_checkBlockFaces(
	in uvec3 _blockUv,
	in uint _chunkId,
	in uint _chunkCount
) {
	// TODO: Semi-transparent blocs
	Block block = getBlock(_blockUv, _chunkCount);

	for (uint face = 0; face < FACE_COUNT; ++face) {
		uint faceMask = 1 << face;
		if ((block.visibleFaces & faceMask) != 0) {
			uint quadIndex = quadCount[_chunkCount]++;
			uint quadPackedData = packQuadData(_blockUv, face, _chunkId);
			quadData[_chunkCount][quadIndex] = quadPackedData;
		}
	}
}

// Check whether or not the chunk's blocks are visible.
void	_checkChunk(in uint _chunkId, in uint _chunkCount) {
	for (uint z = 0; z < CHUNK_SIZE; ++z) {
		for (uint y = 0; y < CHUNK_SIZE; ++y) {
			for (uint x = 0; x < CHUNK_SIZE; ++x) {
				uvec3 blockUv = uvec3(x, y, z);
				_checkBlockFaces(blockUv, _chunkId, _chunkCount);
			}
		}
	}
}

// Frustum culling + occlusion culling
void	_computeChunkCulling(in uvec3 _chunkUvw) {
	uint chunkId = (_chunkUvw.x << 8)
				 + (_chunkUvw.y << 4)
				 + (_chunkUvw.z);
	uint chunkCount = (_chunkUvw.x * RENDER_DISTANCE * RENDER_DISTANCE)
					+ (_chunkUvw.y * RENDER_DISTANCE)
					+  _chunkUvw.z;
	_checkChunk(chunkId, chunkCount);
}

/* ENTRYPOINT =============================================================== */
void main() {
	uvec3 chunkUvw1 = gl_GlobalInvocationID.xyz;
	uvec3 chunkUvw2 = gl_GlobalInvocationID.xyz + uvec3(RENDER_DISTANCE);

	if (_isChunkVisible(_getAABB(chunkUvw1)))
		_computeChunkCulling(chunkUvw1);
	if (_isChunkVisible(_getAABB(chunkUvw2)))
		_computeChunkCulling(chunkUvw2);
}