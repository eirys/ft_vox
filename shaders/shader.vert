#version 450

layout(location = 0) in vec3 vert_position;
layout(location = 1) in vec2 vert_uv;
layout(location = 2) in vec3 vert_normal;
layout(location = 3) in int vert_texture_id;

layout(location = 0) out vec3 frag_normal;
layout(location = 1) out vec2 frag_uv;
layout(location = 2) out int frag_texture_id;

layout(binding = 0) uniform Camera {
	mat4 view;
	mat4 proj;
} camera_ubo;

void	main() {
	frag_normal = vert_normal;
	frag_uv = vert_uv;
	frag_texture_id = vert_texture_id;

	gl_Position =
		camera_ubo.proj
		* camera_ubo.view
		* vec4(vert_position, 1.0f);
}