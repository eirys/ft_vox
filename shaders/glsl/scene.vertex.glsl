#version 450

// Input
layout(location = 0) in int in_position;
// layout(location = 1) in int in_nuvf;

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
		_data & 0xFF,
		(_data >> 8) & 0xFF,
		(_data >> 16) & 0xFF);

	// vec4 height = texture(height_map, vec3(position.xz, gl_InstanceIndex));
	// position.y += int(height.r) & 0xFF;

	vec3 chunk = 16 * vec3(
		gl_InstanceIndex % 5,
		0,
		gl_InstanceIndex / 5);

	// vec3 chunk = 16 * vec3(
	// 	(_data >> 12) & 0xFF,
	// 	(_data >> 20) & 0xF,
	// 	(_data >> 24) & 0xFF);

	return vec4(chunk + position, 1.0);
}

/* MAIN ===================================================================== */
void	main() {
	vec4 position = extractPos(in_position);
	vec3 shadow_coord = (projector.vp * position).xyz;

	// out_normal = extractNormal(in_nuvf);
	out_normal = normals[int(gl_VertexIndex / 4) % 6];
	// out_uvw = vec3(extractUV(in_nuvf), extractTextureIndex(in_nuvf));
	out_uvw = vec3(uvs[int(gl_VertexIndex % 4)], 0);

	out_shadow = vec3(shadow_coord.xy * 0.5f + 0.5f, shadow_coord.z);
	gl_Position = camera.vp * position;
}