#version 450
#define VS

/* ========================================================================== */
/*                                 DESCRIPTORS                                */
/* ========================================================================== */

#define PROJECTOR
#define CHUNKS
#define QUAD_DATA
#define QUAD_COUNT

/* ========================================================================== */
/*                                   MACROS                                   */
/* ========================================================================== */

#include "decl.glslh"
#include "descriptors.glslh"

/* ========================================================================== */
/*                                 SHADER I/O                                 */
/* ========================================================================== */

UNIFORM(SHADOW_SET, 0)					PROJECTOR_DESCRIPTOR;
UNIFORM(SHADOW_SET, 1) usampler2DArray	CHUNKS_DESCRIPTOR;
BUFFER(SHADOW_SET, 2) readonly			QUAD_DATA_DESCRIPTOR;
BUFFER(SHADOW_SET, 3) readonly			QUAD_COUNT_DESCRIPTOR;

/* ========================================================================== */
/*                                  INCLUDES                                  */
/* ========================================================================== */

#include "tools.glslh"

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