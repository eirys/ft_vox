// Vertex shader

struct Input {
    [[vk::location(0)]] int		position: POSITION0;
    [[vk::location(1)]] int		nuvf: TEXCOORD0;
};

struct Output {
						float4				position: SV_Position;
	[[vk::location(0)]] float3				normal: NORMAL0;
	// [[vk::location(1)]] float2				uv:TEXCOORD1;
	// [[vk::location(2)]] nointerpolation int	face: TEXCOORD2;
	[[vk::location(1)]] float4				tex_coord: TEXCOORD1;
};

struct Camera {
    float4x4    view;
    float4x4    proj;
};

// Binding 0: UBO (camera)
cbuffer camera: register(b0) { Camera camera; }

// Binding 3: UBO (projector)
cbuffer projector: register(b3) { Camera projector; }

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

static const float4x4	bias_matrix = {
	0.5,   0,   0, 0.5,
	  0, 0.5,   0, 0.5,
	  0,   0, 0.5, 0.5,
	  0,   0,   0,   1};

// Extract actual vertex position from input
float4	extractPos(int input) {
	// Local position in chunk
	float3	pos = float3(
		input & 0xF,
		(input >> 4) & 0xF,
		(input >> 8) & 0xF);
	
	// Chunk position in world
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
	//output.uv = uvs[(input.nuvf >> 8) & 0xFF];
	// output.face = (input.nuvf >> 16) & 0xFF;
	output.tex_coord = mul(
		mul(bias_matrix, mul(projector.proj, projector.view)),
		position);
	output.position = mul(
		mul(camera.proj, camera.view),
		position);
	
	return output;
}