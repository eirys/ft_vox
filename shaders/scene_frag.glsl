#version 450

// Input
layout(location = 0) in vec3 in_normal;
layout(location = 1) in vec3 in_uvw;
layout(location = 2) in vec3 in_shadow;

// Output
layout(location = 0) out vec4 out_color;

// Uniforms
layout(binding = 2) uniform sampler2D tex_sampler;
layout(binding = 3) uniform sampler2D shadow_sampler;
layout(binding = 4) uniform Light {
	vec3 ambient;
	vec3 vector;
	vec3 color;
	float intensity;
} light;

bool isShadow(vec3 shadow) {
	if (shadow.z > -1.0f && shadow.z < 1.0f) {
		float depth = texture(shadow_sampler, shadow.xy).r;
		return shadow.z + 0.00005f > depth;
	}
	return false;
}

vec4 directionalLighting(
	vec3 shadow_coord,
	vec3 normal,
	vec3 vector,
	vec3 color,
	float intensity
) {
	if (isShadow(shadow_coord))
		return vec4(0.0);

	float illumination = max(dot(normal, vector), 0.0f);
	return vec4(intensity * color * illumination, 1.0f);
}

void main() {
	vec4 color = texture(tex_sampler, in_uvw);

	// Clamp depth value to [0, 1]
	in_shadow.xy = in_shadow.xy * 0.5f + 0.5f;

	vec4 ambient = vec4(light.ambient, 1.0f);
	vec4 directional = directionalLighting(
		in_shadow,
		in_normal,
		light.vector,
		light.color,
		light.intensity);

	out_color = color * (directional + ambient_lighting);
}