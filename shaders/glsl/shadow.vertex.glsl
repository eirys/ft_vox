#version 450

// Input
layout(location = 0) in int in_position;

// Constant buffers
layout(binding = 0, set = 1) uniform Projector { mat4 vp; }	projector;
layout(binding = 1, set = 1) uniform sampler2DArray			height_map;

vec4	extractPos(int _data) {
	vec3 position = vec3(
		_data & 0xF,
		(_data >> 4) & 0xF,
		(_data >> 8) & 0xF);

	float alt = texture(height_map, vec3(position.xz, gl_InstanceIndex)).x;
	vec3 chunk = 16 * vec3(
		gl_InstanceIndex % 5,
		0,
		int(gl_InstanceIndex / 5));

	// vec3 position = vec3(
	// 	in_position & 0xF,
	// 	(in_position >> 4) & 0xF,
	// 	(in_position >> 8) & 0xF);
	// vec3 chunk = 16 * vec3(
	// 	(in_position >> 12) & 0xFF,
	// 	(in_position >> 20) & 0xF,
	// 	(in_position >> 24) & 0xFF);

	return vec4(chunk + position, 1.0);
}

void	main() {
	// vec4 position = extractPos(in_position);

	// gl_Position = projector.vp * position;
	gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
}