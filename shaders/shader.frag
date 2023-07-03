#version 450

layout(location = 0) in vec3 frag_position;
layout(location = 1) in vec3 frag_normal;
layout(location = 2) in vec2 frag_uv;
flat layout(location = 3) in int frag_texture_id;

layout(location = 0) out vec4 frag_color;

layout(binding = 1) uniform samplerCubeArray tex_sampler;
layout(binding = 2) uniform Light {
	vec3 ambient_color;
	vec3 light_vector;
	vec3 spot_color;
	float intensity;
} light_ubo;

void main() {
	// Retrieve the color from the texture using 2D uv and texture id
	vec4 color = texture(tex_sampler, vec4(frag_uv, -5, 0));

	// Apply ambient lighting
 	frag_color = color * vec4(light_ubo.ambient_color, 1.0);

	// Apply directional lighting
	float directional_lighting = max(
		dot(frag_normal, light_ubo.light_vector),
		0.0
	) * light_ubo.intensity;

	frag_color += color * directional_lighting;
}