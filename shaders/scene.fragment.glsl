#version 450

// Input
layout(location = 0) in vec3 in_normal;
layout(location = 1) in vec3 in_uvw;
layout(location = 2) in vec3 in_shadow;

// Output
layout(location = 0) out vec4 out_color;

// Uniforms
layout(binding = 2) uniform Light {
	vec3 ambient;
	vec3 vector;
	vec3 color;
} light;
layout(binding = 3) uniform sampler2DArray tex_sampler;
layout(binding = 4) uniform sampler2D shadow_sampler;

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
	vec3 color
) {
	if (isShadow(shadow_coord))
		return vec4(0.0);

	float illumination = max(dot(normal, vector), 0.0f);
	return vec4(color * illumination, 1.0f);
}

void main() {
	vec4 color = texture(tex_sampler, vec3(in_uvw.xy, in_uvw.z));

	// Clamp depth value to [0, 1]
	vec3 shadow = vec3(in_shadow.xy * 0.5f + 0.5f, in_shadow.z);

	vec4 ambient = vec4(light.ambient, 1.0f);
	vec4 directional = directionalLighting(
		shadow,
		in_normal,
		light.vector,
		light.color);

	out_color = color * (directional + ambient);
}