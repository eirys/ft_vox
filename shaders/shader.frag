#version 450

// Input
layout(location = 0) in vec3 in_normal;
layout(location = 1) in vec4 in_tex;
// layout(location = 2) flat in int in_face;

// Output
layout(location = 0) out vec4 out_color;

// Uniforms
layout(binding = 1) uniform sampler2D tex_sampler;
layout(binding = 2) uniform Light {
	vec3 ambient;
	vec3 vector;
	vec3 color;
	float intensity;
} light;

vec4	retrieveColor(vec4 coord, int index) {
	return texture(tex_sampler, coord.xy / coord.w);
	// if (abs(coord.z) > 1.0)
	// return (vec4)1.0;
}

vec4 directionalLighting(
	vec3 normal,
	vec3 vector,
	vec3 color,
	float intensity
) {
	float illumination = max(dot(normal, vector), 0.0);
	return vec4(
		intensity
		* color
		* max(illumination, 0.0),
		1.0);
}

void main() {
	vec4 color = retrieveColor(in_tex, 0);
	vec4 ambient_lighting = vec4(light.ambient, 1.0);
	vec4 directional_lighting = directionalLighting(
		in_normal,
		light.vector,
		light.color,
		light.intensity);

	out_color = color * (directional_lighting + ambient_lighting);
}