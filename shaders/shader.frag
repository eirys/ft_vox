#version 450

layout(location = 0) in vec3 vert_normal;
layout(location = 1) in vec2 vert_uv;
layout(location = 2) in float vert_texture_id;

layout(location = 0) out vec4 out_color;

layout(binding = 1) uniform sampler2DArray tex_sampler;
layout(binding = 2) uniform LightUbo {
	vec3 ambient_color;
	vec3 light_vector;
	vec3 light_color;
	float light_intensity;
} light;

void main() {
	// Retrieve the color from the texture using 2D uv and texture id
	vec4 color = texture(tex_sampler, vec3(vert_uv, vert_texture_id));

	// Apply ambient lighting
 	out_color = color * vec4(light.ambient_color, 1.0);

	// Apply directional lighting
	float directional_lighting = max(
		dot(vert_normal, light.light_vector),
		0.0
	) * light.light_intensity;

	out_color += color * directional_lighting;
}