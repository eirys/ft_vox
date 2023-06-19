#version 450

layout(location = 0) in vec3 frag_normal;
layout(location = 1) in vec2 frag_uv;
layout(location = 2) flat in int frag_texture_id;

layout(location = 0) out vec4 out_color;

layout(binding = 1) uniform sampler2D tex_sampler;
layout(binding = 2) uniform Light {
	vec3 ambient_color;
	vec3 light_vector;
	vec3 spot_color;
	float intensity;
} light_ubo;

void main() {
	vec4 color = texture(tex_sampler[frag_texture_id], frag_uv);

	// Apply ambient lighting
 	out_color =	color * vec4(light_ubo.ambient_color, 1.0);

	// Apply directional lighting
	float directional_lighting = max(
		dot(frag_normal, light_ubo.light_vector),
		0.0
	) * light_ubo.intensity;

	out_color += color * directional_lighting;
}