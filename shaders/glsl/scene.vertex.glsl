#version 450

/* ========================================================================== */
/*                                   MACROS                                   */
/* ========================================================================== */

#include "../../src/app/generation/chunk_macros.h"

/* ========================================================================== */
/*                                 SHADER I/O                                 */
/* ========================================================================== */

// Input
//  None

// Output
layout(location = 0) out vec3 out_normal;
layout(location = 1) out vec3 out_uvw;
layout(location = 2) out vec3 out_shadow;

/* UNIFORMS ================================================================= */
layout(binding = 0, set = 0) uniform Camera { mat4 vp; }	camera;
layout(binding = 1, set = 0) uniform Projector { mat4 vp; }	projector;
layout(binding = 5, set = 0) uniform usampler2DArray		height_map;
layout(binding = 6, set = 0) uniform usampler2D				chunk_map;

/* ========================================================================== */
/*                                  INCLUDES                                  */
/* ========================================================================== */

#include "scene_rendering_tools.glslh"

/* CONSTS =================================================================== */
const vec3 normals[6] = {
	{  0.0,  1.0,  0.0 },	// 0 top
	{  0.0, -1.0,  0.0 },	// 1 bottom
	{ -1.0,  0.0,  0.0 },	// 2 left
	{  1.0,  0.0,  0.0 },	// 3 right
	{  0.0,  0.0,  1.0 },	// 4 front
	{  0.0,  0.0, -1.0 },	// 5 back
};

const vec2 uvs[6] = {
	{ 0.0, 1.0 },
	{ 1.0, 0.0 },
	{ 1.0, 1.0 },
	{ 0.0, 1.0 },
	{ 0.0, 0.0 },
	{ 1.0, 0.0 },
};

/* MAIN ===================================================================== */
void	main() {
	// Get actual instanceId
	ivec2	instanceUV = ivec2(gl_InstanceIndex % RENDER_DISTANCE, gl_InstanceIndex / RENDER_DISTANCE);	// (x, y) in [0, 5]
	uint	instanceId = texture(chunk_map, instanceUV / vec2(textureSize(chunk_map, 0) - 1)).r; //???

	// Get actual vertexId
	uint	vertexId = gl_VertexIndex % 36;

	// Get cube in chunk
	uint	cubeId = gl_VertexIndex / 36;
	ivec2	chunkPos = ivec2(instanceId % RENDER_DISTANCE, instanceId / RENDER_DISTANCE);
	ivec2	cubePos = ivec2(cubeId % CHUNK_SIZE, cubeId / CHUNK_SIZE);
	float	cubeHeight = getHeight(cubePos, instanceId);
	uint	face = vertexId / 6;

	vec4	position = extractPos(vertexId, cubePos, cubeHeight, chunkPos);
	vec3	shadow_coord = (projector.vp * position).xyz;

	out_shadow = vec3(shadow_coord.xy * 0.5f + 0.5f, shadow_coord.z);
	out_normal = normals[face];
	out_uvw = vec3(uvs[vertexId % 6], face);
	gl_Position = camera.vp * position;
}