// Fragment shader

struct Input {
	[[vk::location(0)]] float3	normal: NORMAL0;
	[[vk::location(1)]] float3	uvw: TEXCOORD0;
	[[vk::location(2)]] float4	shadow: TEXCOORD1;
};

struct Output {
	float4	color: SV_Target;
};

struct Light {
	float3	ambient;
	float3	dir;
	float3	color;
};

cbuffer light: register(b2, space0) { Light light; }

Texture2DArray	tex_map: register(t3, space0);
SamplerState	tex_sampler: register(s3, space0);

Texture2D		shadow_map: register(t4, space0);
SamplerState	shadow_sampler: register(s4, space0);

// Sample shadow
float	filterShadow(float4 shadow_coord) {
	if (shadow_coord.z >= -1.0 && shadow_coord.z <= 1.0) {
		float dist = shadow_map.Sample(shadow_sampler, shadow_coord.xy).r;
		if (dist + 0.00005 /*???*/ < shadow_coord.z)
			return 0.0;
	}
	return 1.0;
}

float4	directionalLighting(float3 normal, float3 light_dir) {
	float illumination = saturate(dot(normal, light_dir));
	return float4((float3)illumination, 1.0);
}

// MAIN FUNCTION
Output	main(Input input) {
	Output	output = (Output)0;

	input.shadow.xy = input.shadow.xy * 0.5 + 0.5;
	float4	color = tex_map.Sample(tex_sampler, input.uvw);
	float4	directional = directionalLighting(input.normal, light.dir);
	float4	ambient = float4(light.ambient, 1.0);
	float	shadow = filterShadow(input.shadow);

	output.color = color * saturate(shadow * directional + ambient);

	return output;
}