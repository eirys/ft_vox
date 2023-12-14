#version 450
#define PS

// Input
layout(location = 0) in vec3 in_normal;
layout(location = 1) in vec3 in_uvw;
layout(location = 2) in vec3 in_shadow;

// Output
layout(location = 0) out vec4 out_color;

/* UNIFORMS ================================================================= */
layout(binding = 2, set = 0) uniform Light {
	vec3 ambient;
	vec3 vector;
	vec3 color;
} light;
layout(binding = 3, set = 0) uniform sampler2DArray	tex_sampler;
layout(binding = 4, set = 0) uniform sampler2D		shadow_sampler;

/* HELPERS ================================================================== */
bool isShadow(vec3 _shadow_coord) {
	if (_shadow_coord.z > 0.0f && _shadow_coord.z < 1.0f) {
		float dist = texture(shadow_sampler, _shadow_coord.xy).r;
		if (dist + 0.001 < _shadow_coord.z)
			return true;
	}
	return false;
}

vec4 directionalLighting(
	vec3 _shadow_coord,
	vec3 _vertex_normal,
	vec3 _light_vector,
	vec3 _light_color
) {
	if (isShadow(_shadow_coord))
		return vec4(0.0);
	float illumination = max(dot(_vertex_normal, _light_vector), 0.0f);
	return vec4(_light_color * illumination, 1.0f);
}

/* ENTRYPOINT =============================================================== */
void main() {
	vec4 color = texture(tex_sampler, in_uvw);
	vec4 ambient = vec4(light.ambient, 1.0f);
	vec4 directional = directionalLighting(
		in_shadow,
		in_normal,
		light.vector,
		light.color);

	out_color = color * (directional + ambient);
}