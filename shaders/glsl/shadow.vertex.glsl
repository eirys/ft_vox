#version 450

/* ========================================================================== */
/*                                 SHADER I/O                                 */
/* ========================================================================== */

// Constant buffers
layout(binding = 0, set = 1) uniform Projector { mat4 vp; }	projector;
layout(binding = 1, set = 1) uniform usampler2DArray		height_map;
layout(binding = 2, set = 1) uniform Packed { uint data; }	packed_position;

/* ========================================================================== */
/*                                  INCLUDES                                  */
/* ========================================================================== */

#include "scene_rendering_tools.glslh"

/* MAIN ===================================================================== */
void	main() {
	int		cubeId = gl_InstanceIndex % CHUNK_AREA;
	int		chunkId = extractChunkId(gl_InstanceIndex / CHUNK_AREA);

	ivec2	cubePos = ivec2(cubeId % 16, cubeId / 16);
	float	cubeHeight = getHeight(cubePos, chunkId);

	vec4 position = extractPos(cubePos, cubeHeight, chunkId);
	gl_Position = projector.vp * position;
}