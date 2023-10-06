#version 450

/* ========================================================================== */
/*                                 SHADER I/O                                 */
/* ========================================================================== */

// Input
// layout(location = 0) in int in_position;

// Output
layout(location = 0) out vec3 out_normal;
layout(location = 1) out vec3 out_uvw;
layout(location = 2) out vec3 out_shadow;

/* UNIFORMS ================================================================= */
layout(binding = 0, set = 0) uniform Camera { mat4 vp; }	camera;
layout(binding = 1, set = 0) uniform Projector { mat4 vp; }	projector;
layout(binding = 5, set = 0) uniform usampler2DArray		height_map;

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
	int		cubeId = gl_InstanceIndex % CHUNK_AREA;
	int		chunkId = gl_InstanceIndex / CHUNK_AREA;

	ivec2	cubePos = ivec2(cubeId % 16, cubeId / 16);
	float	cubeHeight = getHeight(cubePos, chunkId);
	int		face = gl_VertexIndex / 6;

	if (cullUnseenFace(face, cubePos, chunkId, cubeHeight)) {
		gl_Position = vec4(0.0f, 0.0f, 0.0f, 1.0f);
		return;
	}

	vec4 position = extractPos(cubePos, cubeHeight, chunkId);
	vec3 shadow_coord = (projector.vp * position).xyz;

	out_shadow = vec3(shadow_coord.xy * 0.5f + 0.5f, shadow_coord.z);
	out_normal = normals[face];
	out_uvw = vec3(uvs[gl_VertexIndex % 6], face);
	gl_Position = camera.vp * position;
}