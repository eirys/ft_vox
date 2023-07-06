#version 450

layout(location = 0) in vec3 in_normal;
layout(location = 1) in vec2 in_uv;
layout(location = 2) flat in int in_face;

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
	vec4 color = texture(tex_sampler, vec3(in_uv, in_face));

	// Apply ambient lighting
 	out_color = color * vec4(light.ambient_color, 1.0);

	// Apply directional lighting
	float directional_lighting = max(
		dot(in_normal, light.light_vector),
		0.0
	) * light.light_intensity;

	out_color += color * directional_lighting;
}