#version 450

// Input
layout(location = 0) in int in_position;
layout(location = 1) in int in_nuvf;

// Output
layout(location = 0) out vec3 out_normal;
layout(location = 1) out vec3 out_uvw;
layout(location = 2) out vec3 out_shadow;

// Uniforms
layout(binding = 0) uniform Camera { mat4 vp; } camera;
layout(binding = 1) uniform Projector { mat4 vp; } projector;

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

vec4	extractPos(int in_position) {
	vec3 position = vec3(
		in_position & 0xF,
		(in_position >> 4) & 0xF,
		(in_position >> 8) & 0xF);

	vec3 chunk = 16 * vec3(
		(in_position >> 12) & 0xFF,
		(in_position >> 20) & 0xF,
		(in_position >> 24) & 0xFF);

	return vec4(chunk + position, 1.0);
}

void	main() {
	vec4 position = extractPos(in_position);

	out_normal = normals[in_nuvf & 0xFF];
	out_uvw = vec3(
		uvs[(in_nuvf >> 8) & 0xFF],	// uv
		(in_nuvf >> 16) & 0xFF);	// face
	out_shadow = (projector.vp * position).xyz;
	gl_Position = camera.vp * position;
}