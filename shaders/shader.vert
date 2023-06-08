#version 450

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_color;
layout(location = 2) in vec2 in_tex_coord;
layout(location = 3) in vec3 in_normal;

layout(location = 0) out vec3 frag_normal;
layout(location = 1) out vec2 frag_tex_coord;


layout(binding = 0) uniform Camera {
	mat4 view;
	mat4 proj;
} camera_ubo;

void	main() {
	frag_normal = in_normal;
	frag_tex_coord = in_tex_coord;

	gl_Position =
		camera_ubo.proj
		* camera_ubo.view
		* vec4(in_position, 1.0f);
}