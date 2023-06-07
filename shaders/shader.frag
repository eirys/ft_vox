#version 450

layout(location = 0) in vec3 frag_color;

layout(location = 0) out vec4 out_color;

// Reference to combined image sampler descriptor
layout(binding = 1) uniform sampler2D tex_sampler;

void main() {
	out_color = vec4(frag_color, 1.0);
	// out_color = texture(tex_sampler, frag_tex_coord);
}