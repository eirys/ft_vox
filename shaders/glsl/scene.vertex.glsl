#version 450

/* ========================================================================== */
/*                                 SHADER I/O                                 */
/* ========================================================================== */

// Input
layout(location = 0) in int in_position;

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
	{  0.0f,  1.0f,  0.0f },	// 0 top
	{  0.0f, -1.0f,  0.0f },	// 1 bottom
	{ -1.0f,  0.0f,  0.0f },	// 2 left
	{  1.0f,  0.0f,  0.0f },	// 3 right
	{  0.0f,  0.0f,  1.0f },	// 4 front
	{  0.0f,  0.0f, -1.0f },	// 5 back
};

const vec2 uvs[4] = {
	{ 0.0f, 1.0f },
	{ 1.0f, 1.0f },
	{ 1.0f, 0.0f },
	{ 0.0f, 0.0f }
};

/* MAIN ===================================================================== */
void	main() {
	int side = int(gl_VertexIndex / 4) % 6;
	int cube_id = gl_VertexIndex / 24;

	// Right or front: potential culling
	if (side == 3) {

	} else if (side == 4) {

	} else {
		ivec2	cube_pos = ivec2(cube_id % 16, cube_id / 16);
		float	height = getHeight(cube_pos, gl_InstanceIndex);

		out_normal = normals[side];
		out_uvw = vec3(uvs[gl_VertexIndex % 4], side);

		vec4 position = extractPos(in_position, height);
		vec3 shadow_coord = (projector.vp * position).xyz;

		out_shadow = vec3(shadow_coord.xy * 0.5f + 0.5f, shadow_coord.z);

		gl_Position = camera.vp * position;
	}
}