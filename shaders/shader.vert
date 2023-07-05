#version 450

layout(location = 0) in int in_position;
layout(location = 1) in int in_nuvf;

layout(location = 0) out int nuvf;

layout(binding = 0) uniform Camera {
	mat4 view;
	mat4 proj;
} camera_ubo;

void	main() {
	// Extract vec3 position from float
	vec4 position = vec4(
		in_position & 0xFF,
		(in_position >> 8) & 0xFF,
		(in_position >> 16) & 0xFF,
		1.0f
	);

	nuvf = in_nuvf;
	gl_Position =
		camera_ubo.proj
		* camera_ubo.view
		* position;
}