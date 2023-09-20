// Vertex shader

struct Input {
	[[vk::location(0)]] int		position: POSITION0;
};

struct Output {
						float4	position: SV_Position;
};

struct Camera {
	float4x4    vp;
};

cbuffer projector: register(b0, space1) { Camera projector; }

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

	output.position = mul(projector.vp, position);

	return output;
}