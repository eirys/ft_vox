#version 450
#define PS

/* ========================================================================== */
/*                                   MACROS                                   */
/* ========================================================================== */

#include "decl.glslh"

/* ========================================================================== */
/*                                 SHADER I/O                                 */
/* ========================================================================== */

// Input
INPUT(0)			vec3 in_normal;
INPUT(1)			vec3 in_uvw;
INPUT(2)			vec3 in_shadow;

// layout(location = 0) in vec3 in_normal;
// layout(location = 1) in vec3 in_uvw;
// layout(location = 2) in vec3 in_shadow;

// Output
OUTPUT(0)			vec4 out_color;

// layout(location = 0) out vec4 out_color;

/* UNIFORMS ================================================================= */
UNIFORM(SCENE_SET, 2) Light {
	vec3 ambient;
	vec3 vector;
	vec3 color;
} light;

UNIFORM(SCENE_SET, 3) sampler2DArray	tex_sampler;
UNIFORM(SCENE_SET, 4) sampler2D			shadow_sampler;

// layout(binding = 2, set = 0) uniform Light {
// 	vec3 ambient;
// 	vec3 vector;
// 	vec3 color;
// } light;
// layout(binding = 3, set = 0) uniform sampler2DArray	tex_sampler;
// layout(binding = 4, set = 0) uniform sampler2D		shadow_sampler;

/* HELPERS ================================================================== */
bool _isShadow(vec3 _shadow_coord) {
	if (_shadow_coord.z > 0.0 && _shadow_coord.z < 1.0) {
		float dist = texture(shadow_sampler, _shadow_coord.xy).r;
		if (dist + 0.001 < _shadow_coord.z)
			return true;
	}
	return false;
}

vec4 _computeDirectionalLighting(
	vec3 _shadow_coord,
	vec3 _vertex_normal,
	vec3 _light_vector,
	vec3 _light_color
) {
	if (_isShadow(_shadow_coord))
		return vec4(0.0);
	float illumination = max(dot(_vertex_normal, _light_vector), 0.0);
	return vec4(_light_color * illumination, 1.0);
}

/* ENTRYPOINT =============================================================== */
void main() {
	vec4 color = texture(tex_sampler, in_uvw);
	vec4 ambient = vec4(light.ambient, 1.0);
	vec4 directional = _computeDirectionalLighting(
		in_shadow,
		in_normal,
		light.vector,
		light.color);

	out_color = color * (directional + ambient);
}