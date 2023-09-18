// Vertex shader

struct Input {
	[[vk::location(0)]] int		position: POSITION0;
	[[vk::location(1)]] int		nuvf: TEXCOORD0;
};

struct Output {
						float4	position: SV_Position;
	[[vk::location(0)]] float3	normal: NORMAL0;
	[[vk::location(1)]] float3	uvw: TEXCOORD0;
	[[vk::location(2)]] float4	shadow: TEXCOORD1;
};

struct Camera {
	float4x4    vp;
};

cbuffer camera: register(b0, space0) { Camera camera; }

cbuffer projector: register(b1, space0) { Camera projector; }

// Normals
static const float3 normals[6] = {
	{  0.0f,  0.0f,  1.0f },	// front
	{  0.0f,  0.0f, -1.0f },	// back
	{  1.0f,  0.0f,  0.0f },	// right
	{ -1.0f,  0.0f,  0.0f },	// left
	{  0.0f,  1.0f,  0.0f },	// top
	{  0.0f, -1.0f,  0.0f }};	// bottom

// Uvs
static const float2	uvs[4] = {
	{ 0.0f, 1.0f },
	{ 1.0f, 1.0f },
	{ 1.0f, 0.0f },
	{ 0.0f, 0.0f }};

// Extract actual vertex position from input
float4	extractPos(int input) {
	float3	pos = float3(
		input & 0xF,
		(input >> 4) & 0xF,
		(input >> 8) & 0xF);

	float3	chunk = 16 * float3(
		(input >> 12) & 0xFF,
		(input >> 20) & 0xF,
		(input >> 24) & 0xFF);

	return float4(
		chunk + pos,
		1.0
	);
}

// MAIN FUNCTION
Output	main(Input input) {
	Output	output = (Output)0;
	float4	position = extractPos(input.position);

	output.normal = normals[input.nuvf & 0xFF];
	output.uvw = float3(
		uvs[(input.nuvf >> 8) & 0xFF],	// uv coord
		(input.nuvf >> 16) & 0xFF);		// face index as w
	output.shadow = mul(projector.vp, position);
	output.position = mul(camera.vp, position);

	return output;
}