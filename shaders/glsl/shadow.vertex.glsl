#version 450

/* ========================================================================== */
/*                                 SHADER I/O                                 */
/* ========================================================================== */

// Input
// layout(location = 0) in int in_position;

// Constant buffers
layout(binding = 0, set = 1) uniform Projector { mat4 vp; }	projector;
layout(binding = 1, set = 1) uniform usampler2DArray		height_map;

/* ========================================================================== */
/*                                  INCLUDES                                  */
/* ========================================================================== */

#include "scene_rendering_tools.glslh"

/* MAIN ===================================================================== */
void	main() {
	gl_Position= vec4(0.0, 0.0, 0.0, 1.0);
	// int side = int(gl_VertexIndex / 4) % 6;
	// int		cube_id = gl_VertexIndex / 24;
	// ivec2	cube_pos = ivec2(cube_id % 16, cube_id / 16);
	// float	height = getHeight(cube_pos, gl_InstanceIndex);
	// vec4 position = extractPos(in_position, height);

	// gl_Position = projector.vp * position;
}