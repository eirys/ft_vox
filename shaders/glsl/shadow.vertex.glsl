#version 450

#include "../../src/app/generation/chunk_macros.h"

// Input
layout(location = 0) in int in_position;

// Constant buffers
layout(binding = 0, set = 1) uniform Projector { mat4 vp; }	projector;
layout(binding = 1, set = 1) uniform usampler2DArray			height_map;

vec4	extractPos(int _data) {
	vec3 position = vec3(
		_data & 0xF,
		(_data >> 4) & 0xF,
		(_data >> 8) & 0xF);

	int cube_id = gl_VertexIndex / 24;
	ivec2 cube_pos = ivec2(cube_id % 16, cube_id / 16);							// 0 to 255
	vec2 cube_pos_remap = vec2(cube_pos) / vec2(textureSize(height_map, 0));	// Remap to 0 - 1

	position.y += texture(height_map, vec3(cube_pos_remap, gl_InstanceIndex)).r;

	vec3 chunk = 16.0 * vec3(
		gl_InstanceIndex % RENDER_DISTANCE,
		0,
		gl_InstanceIndex / RENDER_DISTANCE);

	return vec4(chunk + position, 1.0);
}

void	main() {
	// vec4 position = extractPos(in_position);

	// gl_Position = projector.vp * position;
	gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
}