#version 450

layout(location = 0) in int in_position;
layout(location = 1) in int in_nuvf;

layout(location = 0) out vec3 out_normal;
layout(location = 1) out vec4 out_tex;
// layout(location = 1) out vec2 out_uv;
// layout(location = 2) out int out_face;

layout(binding = 0) uniform Camera {
	mat4 view;
	mat4 proj;
} camera;

layout(binding = 3) uniform Projector {
	mat4 view;
	mat4 proj;
} projector;

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
const mat4	bias_matrix = {
	{ 0.5,   0,   0, 0.5 },
	{   0, 0.5,   0, 0.5 },
	{   0,   0, 0.5, 0.5 },
	{   0,   0,   0,   1 }};

// Extract local position
vec4	extractPos(int in_position) {
	vec3 position = vec3(
		in_position & 0xF,
		(in_position >> 4) & 0xF,
		(in_position >> 8) & 0xF);

	// Extract chunk address
	vec3 chunk = 16 * vec3(
		(in_position >> 12) & 0xFF,
		(in_position >> 20) & 0xF,
		(in_position >> 24) & 0xFF);

	return vec4(chunk + position, 1.0);
}

void	main() {
	vec4 position = extractPos(in_position);

	out_normal = normals[in_nuvf & 0xFF];
	out_tex =
		bias_matrix
		* projector.proj
		* projector.view
		* position;

	gl_Position =
		camera.proj
		* camera.view
		* position;
}