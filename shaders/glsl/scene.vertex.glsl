#version 450

#include "../../src/app/generation/chunk_macros.h"

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
// layout(r8ui, binding = 5, set = 0) uniform readonly uimage2DArray			height_map;

/* CONSTS =================================================================== */
const vec3 normals[6] = {
	{  0.0f,  0.0f,  1.0f },	// front
	{  0.0f,  0.0f, -1.0f },	// back
	{  1.0f,  0.0f,  0.0f },	// right
	{ -1.0f,  0.0f,  0.0f },	// left
	{  0.0f,  1.0f,  0.0f },	// top
	{  0.0f, -1.0f,  0.0f }};	// bottom

const vec2 uvs[4] = {
	{ 0.0f, 1.0f },
	{ 1.0f, 1.0f },
	{ 1.0f, 0.0f },
	{ 0.0f, 0.0f }
};

/* HELPERS ================================================================== */

vec4	extractPos(int _data) {
	vec3 position = vec3(
		(_data >> 16)	& 0xFF,
		(_data >> 8)	& 0xFF,
		_data 			& 0xFF);

	int cube_id = gl_VertexIndex / 24;
	ivec2 cube_pos = ivec2(cube_id % 16, cube_id / 16);							// 0 to 255
	vec2 cube_pos_remap = vec2(cube_pos) / vec2(textureSize(height_map, 0));	// Remap to 0 - 1

	position.y += texture(height_map, vec3(cube_pos_remap, gl_InstanceIndex)).r;
	// position.y += texelFetch(height_map, ivec3(cube_pos, gl_InstanceIndex), 0).r;

	vec3 chunk = 16.0 * vec3(
		gl_InstanceIndex % RENDER_DISTANCE,
		0,
		gl_InstanceIndex / RENDER_DISTANCE);

	return vec4(chunk + position, 1.0);
}

/* MAIN ===================================================================== */
void	main() {
	vec4 position = extractPos(in_position);
	vec3 shadow_coord = (projector.vp * position).xyz;

	int side = int(gl_VertexIndex / 4) % 6;
	out_normal = normals[side];
	out_uvw = vec3(uvs[gl_VertexIndex % 4], side);
	out_shadow = vec3(shadow_coord.xy * 0.5f + 0.5f, shadow_coord.z);

	gl_Position = camera.vp * position;
}