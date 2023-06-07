#version 450

layout(location = 0) in vec3 pos_world;
layout(location = 1) in vec3 normal_world;

layout(location = 0) out vec4 out_color;

layout(binding = 1) uniform sampler2D tex_sampler;
layout(binding = 2) uniform Light {
	vec3 ambient_color;
	vec3 light_vector;
	vec3 spot_color;
	float intensity;
} light_ubo;

const vec4 _gray_scale = vec4(0.7, 0.7, 0.7, 1.0);

void main() {
	// Apply ambient lighting
 	out_color =	_gray_scale * vec4(light_ubo.ambient_color, 1.0);

	// Apply directional lighting
	float directional_lighting = max(
		dot(normal_world, light_ubo.light_vector),
		0.0
	) * light_ubo.intensity;
	out_color += _gray_scale * directional_lighting;
}