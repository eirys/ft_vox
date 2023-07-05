#version 450

layout(location = 0) flat in int nuvf;

layout(location = 0) out vec4 out_color;

layout(binding = 1) uniform sampler2DArray tex_sampler;
layout(binding = 2) uniform LightUbo {
	vec3 ambient_color;
	vec3 light_vector;
	vec3 light_color;
	float light_intensity;
} light;

const vec3 normals[6] = {
	{0.0f, 0.0f, 1.0f}, // front
	{0.0f, 0.0f, -1.0f}, // back
	{1.0f, 0.0f, 0.0f}, // right
	{-1.0f, 0.0f, 0.0f}, // left
	{0.0f, 1.0f, 0.0f}, // top
	{0.0f, -1.0f, 0.0f} // bottom
};

const vec2 uvs[4] = {
	{0.0f, 1.0f},
	{1.0f, 1.0f},
	{1.0f, 0.0f},
	{0.0f, 0.0f}
};

void main() {
	// Extract the normal, uv and texture id from the nuvf
	vec3 vert_normal = normals[nuvf & 0xFF];
	vec2 vert_uv = uvs[(nuvf >> 8) & 0xFF];
	float vert_texture_id = (nuvf >> 16) & 0xFF;

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