#version 450

layout(location = 0) in int in_position;
layout(location = 1) in int in_nuvf;

layout(location = 0) out vec3 out_normal;
layout(location = 1) out vec2 out_uv;
layout(location = 2) out int out_face;

layout(binding = 0) uniform Camera {
	mat4 view;
	mat4 proj;
} camera_ubo;

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

void	main() {
	// Extract vec3 position from float
	vec4 position = vec4(
		in_position & 0xFF,
		int(float(in_position) / 0x100) & 0xFF,
		int(float(in_position) / 0x10000) & 0xFF,
		1.0f
	);

	// Extract the normal, uv and texture id from the nuvf
	out_normal = normals[in_nuvf & 0xFF];
	out_uv = uvs[int(float(in_nuvf) / 0x100) & 0xFF];
	out_face = int(float(in_nuvf) / 0x10000) & 0xFF;

	gl_Position =
		camera_ubo.proj
		* camera_ubo.view
		* position;
}