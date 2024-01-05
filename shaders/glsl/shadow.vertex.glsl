#version 450
#define VS

/* ========================================================================== */
/*                                   MACROS                                   */
/* ========================================================================== */

#include "../../src/app/generation/chunk_macros.h"
#include "decl.glslh"

/* ========================================================================== */
/*                                 SHADER I/O                                 */
/* ========================================================================== */

// Constant buffers
// layout(binding = 0, set = 1) uniform Projector { mat4 vp; }	projector;
// layout(binding = 1, set = 1) uniform usampler2DArray		height_map;
// layout(binding = 2, set = 1) uniform usampler2D				chunk_map;

UNIFORM(SHADOW_SET, 0)			Projector { mat4 vp; }	projector;
UNIFORM(SHADOW_SET, 1)			usampler2DArray			chunks;
BUFFER(SHADOW_SET, 2, std140)	VerticesData {
	uint vertexData[MAX_RENDER_PYRAMID];

	// uint vertexData[MAX_RENDER_DISTANCE * CHUNK_VOLUME * FACE_COUNT];
	// blockId: 12 bit		-> to retrieve block type in `chunks` + block position
	// face: 4 bits			-> to retrieve which face to sample
	// layer: 12 bits		-> to retrieve which chunk to offset block position
	// orientation: 1 bit	-> swap uv orientation for directed faces
	// unused: 3 bits
} vertices_data;

/* ========================================================================== */
/*                                  INCLUDES                                  */
/* ========================================================================== */

#include "scene_rendering_tools.glslh"

/* ENTRYPOINT =============================================================== */
void	main() {
	gl_Position = vec4(0.0);
	// int		cubeId = gl_InstanceIndex % CHUNK_AREA;
	// int		instanceId = gl_InstanceIndex / RENDER_DISTANCE;
	// int		chunkId = int(texture(chunk_map, float(instanceId) / textureSize(chunk_map, 0)).r);

	// ivec2	chunkPos = ivec2(chunkId % RENDER_DISTANCE, chunkId / RENDER_DISTANCE);
	// ivec2	cubePos = ivec2(cubeId % 16, cubeId / 16);
	// float	cubeHeight = getHeight(cubePos, chunkId);
	// int		face = gl_VertexIndex / 6;

	// if (cullUnseenFace(face, cubePos, chunkId, cubeHeight)) {
	// 	gl_Position = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	// 	return;
	// }

	// vec4	position = extractPos(cubePos, cubeHeight, chunkPos);
	// gl_Position = projector.vp * position;
}