#version 450

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;
layout(location = 3) in int in_texture_id;

layout(location = 0) out vec3 vert_normal;
layout(location = 1) out vec2 vert_uv;
layout(location = 2) out float vert_texture_id;

layout(binding = 0) uniform Camera {
	mat4 view;
	mat4 proj;
} camera_ubo;

void	main() {
	vert_normal = in_normal;
	vert_uv = in_uv;
	vert_texture_id = in_texture_id;
	gl_Position =
		camera_ubo.proj
		* camera_ubo.view
		* vec4(in_position, 1.0f);
}