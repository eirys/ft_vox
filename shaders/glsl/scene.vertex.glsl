#version 450

// Input
layout(location = 0) in int in_position;

// Output
layout(location = 0) out vec3 out_normal;
layout(location = 1) out vec3 out_uvw;
layout(location = 2) out vec3 out_shadow;

/* UNIFORMS ================================================================= */
layout(binding = 0, set = 0) uniform Camera { mat4 vp; }	camera;
layout(binding = 1, set = 0) uniform Projector { mat4 vp; }	projector;
layout(binding = 5, set = 0) uniform sampler2DArray			height_map;

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
		(_data >> 16) & 0xFF,
		(_data >> 8) & 0xFF,
		_data & 0xFF);

	// A layer of height_map = a chunk
	// vec4 height = texture(height_map, vec3(position.xz, gl_InstanceIndex));

	int cube_id = gl_VertexIndex / 24; // cube id in chunk
	ivec2 cube_pos = ivec2(cube_id & 0xF0, cube_id & 0x0F);

	position.y += texelFetch(height_map, ivec3(cube_pos, gl_InstanceIndex), 0).r * 255.0;

	vec3 chunk = 16 * vec3(
		gl_InstanceIndex % 5,
		0,
		gl_InstanceIndex / 5);

	return vec4(chunk + position, 1.0);
}

/* MAIN ===================================================================== */
void	main() {
	vec4 position = extractPos(in_position);
	vec3 shadow_coord = (projector.vp * position).xyz;

	int side = int(gl_VertexIndex / 4) % 6;
	out_normal = normals[side];
	out_uvw = vec3(uvs[int(gl_VertexIndex % 4)], side);
	// out_shadow = vec3(shadow_coord.xy * 0.5f + 0.5f, shadow_coord.z);

	gl_Position = camera.vp * position;
}