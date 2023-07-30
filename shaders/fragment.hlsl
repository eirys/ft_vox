// Fragment shader

struct Input {
	[[vk::location(0)]] float3	normal: NORMAL0;
	// [[vk::location(1)]] float2	uv:TEXCOORD1;
	// int		face:TEXCOORD2;
	[[vk::location(1)]] float4	tex_coord: TEXCOORD1;
};

struct Output {
	float4	color: SV_Target;
};

struct Light {
	float3	ambient;
	float3	color;
	float3	dir;
	float	intensity;
};

// Binding 1: Sampler
Texture2D		tex_sampler: register(t1);
SamplerState	state_sampler: register(s1);

// Binding 2: UBO (light)
cbuffer light: register(b2) { Light light; }

// Sample texture. Texture proj.
float4	retrieveColor(float4 coord, int index) {
	if (abs(coord.z) > 1.0)
		return tex_sampler.Sample(
			state_sampler,
			coord.xy / coord.w
		);
	return (float4)1.0;
}

// Compute lighting
float	directionalLighting(
	float3 normal,
	float3 light_dir,
	float intensity
) {
	return
		max(dot(normal,light_dir), 0.0)
		* intensity;
}

// MAIN FUNCTION
Output	main(Input input) {
	Output	output = (Output)0;

	float4 color = retrieveColor(input.tex_coord, 0);
	float4 ambient = float4(light.ambient, 1.0);
	float4 directional = float4(
		directionalLighting(
			input.normal,
			light.dir,
			light.intensity),
		1.0);
	output.color = color * (ambient + directional)

	return output;
}