// Vertex shader

struct Input {
    [[vk::location(0)]] int		position: POSITION0;
    [[vk::location(1)]] int		nuvf: TEXCOORD0;
};

struct Output {
						float4	position: SV_Position;
	[[vk::location(0)]] float3	normal: NORMAL0;
	[[vk::location(1)]] float2	uv: TEXCOORD1;
	[[vk::location(2)]] int		face: TEXCOORD2;
};

// Binding 0: UBO (camera)
struct Camera {
    float4x4    view;
    float4x4    proj;
};

cbuffer camera: register(b0) { Camera camera; }

// Normals
static const float3 normals[6] = {
    {0.0f, 0.0f, 1.0f}, // front
    {0.0f, 0.0f, -1.0f}, // back
    {1.0f, 0.0f, 0.0f}, // right
    {-1.0f, 0.0f, 0.0f}, // left
    {0.0f, 1.0f, 0.0f}, // top
    {0.0f, -1.0f, 0.0f}}; // bottom

// Uvs
static const float2	uvs[4] = {
	{0.0f, 1.0f},
	{1.0f, 1.0f},
	{1.0f, 0.0f},
	{0.0f, 0.0f}};

// Extract actual vertex position from input
float4	extractPos(int input) {
	// Local position in chunk
	float3	pos = float3(
		input & 0xF,
		(input >> 4) & 0xF,
		(input >> 8) & 0xF);
	
	// Chunk position in world
	int chunk_x = (input >> 12) & 0xFF;
	int chunk_z = (input >> 20) & 0xFF;
	int chunk_y = (input >> 28) & 0xF;
	
	return float4(
		(chunk_x * 16) + pos.x,
		(chunk_y * 16) + pos.y,
		(chunk_z * 16) + pos.z,
		1.0
	);
}

// MAIN FUNCTION
Output	main(Input input) {
	Output	output = (Output)0;
	
	output.normal = normals[input.nuvf & 0xFF];
	output.uv = uvs[int(float(input.nuvf) / 0x100) & 0xFF];
	output.face = int(float(input.nuvf) / 0x10000) & 0xFF;
	output.position = mul(
		camera.proj,
		mul(camera.view, extractPos(input.position))
	);
	
	return output;
}