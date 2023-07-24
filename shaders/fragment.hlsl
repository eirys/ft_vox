// Fragment shader

struct Input {
	float3				normal: NORMAL0;
	float2				uv: TEXCOORD1;
	nointerpolation int	face: TEXCOORD2;
};

struct Output {
	float4	color: SV_Target;
};

// Binding 1: Sampler
Texture2DArray	tex_sampler: register(t1);
SamplerState	sampler_array: register(s1);

// Binding 2: UBO (light)
struct Light {
	float3	ambient;
	float3	color;
	float3	dir;
	float	intensity;
};

cbuffer light: register(b2) { Light light; }

// Sample texture
float4	retrieveColor(float2 uv, int index) {
	return tex_sampler.Sample(
		sampler_array,
		float3(uv, index));
}

// Compute lighting
float4	directionalLighting(
	float3 normal,
	float3 light_dir,
	float intensity
) {
	return
		intensity *
		max(dot(normal, light_dir), 0.0);
}

// MAIN FUNCTION
Output	main(Input input) {
	Output	output = (Output)0;
	
	output.color = retrieveColor(input.uv, input.face) * (
		directionalLighting(input.normal, light.dir, light.intensity) +
		float4(light.ambient, 1.0));

	return output;
}