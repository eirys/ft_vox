#version 450

// Input
layout(location = 0) in int in_position;

// Constant buffers
layout(binding = 0, set = 1) uniform Projector { mat4 vp; } projector;

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

	gl_Position = projector.vp * position;
}