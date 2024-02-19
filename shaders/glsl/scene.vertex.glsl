#version 450
#define VS

/* ========================================================================== */
/*                                 DESCRIPTORS                                */
/* ========================================================================== */

#define CAMERA
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

// Output
OUTPUT(0)						vec3 out_normal;
OUTPUT(1)						vec3 out_uvw;
OUTPUT(2)						vec3 out_shadow;

/* UNIFORMS ================================================================= */
UNIFORM(SCENE_SET, 0)					CAMERA_DESCRIPTOR;
UNIFORM(SCENE_SET, 1)					PROJECTOR_DESCRIPTOR;
UNIFORM(SCENE_SET, 5) usampler2DArray	CHUNKS_DESCRIPTOR;
BUFFER(SCENE_SET, 6) readonly			QUAD_DATA_DESCRIPTOR;
BUFFER(SCENE_SET, 7) readonly			QUAD_COUNT_DESCRIPTOR;

/* ========================================================================== */
/*                                  INCLUDES                                  */
/* ========================================================================== */

#include "tools.glslh"

/* CONSTS =================================================================== */
const vec3 normals[FACE_COUNT] = {
	{  0.0,  1.0,  0.0 },	// 0 top
	{  0.0, -1.0,  0.0 },	// 1 bottom
	{ -1.0,  0.0,  0.0 },	// 2 left
	{  1.0,  0.0,  0.0 },	// 3 right
	{  0.0,  0.0,  1.0 },	// 4 front
	{  0.0,  0.0, -1.0 },	// 5 back
};

const vec2 uvs[FACE_COUNT] = {
	{ 0.0, 1.0 },
	{ 1.0, 0.0 },
	{ 1.0, 1.0 },
	{ 0.0, 1.0 },
	{ 0.0, 0.0 },
	{ 1.0, 0.0 },
};

/* ENTRYPOINT =============================================================== */


void	main() {
	QuadInput	quadInput = _getQuadInput();
	Block 		block = getBlock(quadInput.blockUv, quadInput.layer);

	// DEPRECATED
	// Get actual instanceId
	// ivec2	instanceUV = ivec2(gl_InstanceIndex % RENDER_DISTANCE, gl_InstanceIndex / RENDER_DISTANCE);	// (x, y) in [0, 5]
	// uint	instanceId = texture(chunk_map, instanceUV / vec2(textureSize(chunk_map, 0) - 1)).r; //???

	// // Get actual vertexId
	// uint	vertexId = gl_VertexIndex % 36;

	// // Get cube in chunk
	// uint	cubeId = gl_VertexIndex / 36;
	// ivec2	chunkPos = ivec2(instanceId % RENDER_DISTANCE, instanceId / RENDER_DISTANCE);
	// ivec2	cubePos = ivec2(cubeId % CHUNK_SIZE, cubeId / CHUNK_SIZE);
	// float	cubeHeight = getHeight(cubePos, instanceId);
	// uint	face = vertexId / FACE_COUNT;

	// vec4	position = extractPos(vertexId, cubePos, cubeHeight, chunkPos);

	vec4	worldPos = vec4(block.worldPos, 1.0);
	vec3	shadow_coord = (projector * worldPos).xyz;

	out_shadow = vec3(shadow_coord.xy * 0.5f + 0.5f, shadow_coord.z);
	out_normal = normals[quadInput.face];
	out_uvw = vec3(uvs[quadInput.face], clamp(quadInput.face, 0, 2));
	gl_Position = camera * worldPos;
}