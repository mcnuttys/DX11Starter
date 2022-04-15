#include "ShaderIncludes.hlsli"
#include "Lighting.hlsli"

#define NUM_LIGHTS 5

cbuffer ExternalData : register(b0)
{
	float roughness;
	float2 UVScale;
	float2 UVOffset;
	float3 colorTint;
	float3 cameraPosition; 
	float3 ambient;

	Light lights[NUM_LIGHTS];
}

Texture2D Albedo			: register(t0);
Texture2D NormalMap			: register(t1);
Texture2D RoughnessMap		: register(t2);
Texture2D MetalnessMap		: register(t3);
SamplerState BasicSampler	: register(s0);


float4 main(VertexToPixel input) : SV_TARGET
{ 
	input.uv = input.uv * UVScale + UVOffset;

	input.normal = normalize(input.normal);
	input.tangent = normalize(input.tangent);

	float3 unpackedNormal = NormalMap.Sample(BasicSampler, input.uv).rgb * 2 - 1;
	
	// Feel free to adjust/simplify this code to fit with your existing shader(s)
	// Simplifications include not re-normalizing the same vector more than once!
	float3 N = normalize(input.normal); // Must be normalized here or before
	float3 T = normalize(input.tangent); // Must be normalized here or before
	T = normalize(T - N * dot(T, N)); // Gram-Schmidt assumes T&N are normalized!
	float3 B = cross(T, N);
	float3x3 TBN = float3x3(T, B, N);

	// Assumes that input.normal is used later in the shader
	input.normal = mul(unpackedNormal, TBN); // Note multiplication order!

	float roughness = RoughnessMap.Sample(BasicSampler, input.uv).r;
	float metalness = MetalnessMap.Sample(BasicSampler, input.uv).r;

	// Specular color determination -----------------
	// Assume albedo texture is actually holding specular color where metalness == 1
	//
	// Note the use of lerp here - metal is generally 0 or 1, but might be in between
	// because of linear texture sampling, so we lerp the specular color to match

	float3 surfaceColor = Albedo.Sample(BasicSampler, input.uv).rgb;
	surfaceColor.rgb = pow(surfaceColor.rgb, 2.2);

	float3 specularColor = lerp(F0_NON_METAL.rrr, surfaceColor.rgb, metalness);

	float3 totalLight = ambient * surfaceColor.rgb;

	for (int i = 0; i < NUM_LIGHTS; i++) {
		Light light = lights[i];
		light.Direction = normalize(light.Direction);

		switch (light.Type) {
		case LIGHT_TYPE_DIRECTIONAL:
			totalLight += DirLightPBR(light, input.normal, input.worldPosition, cameraPosition, roughness, metalness, surfaceColor.rgb, specularColor);
			break;

		case LIGHT_TYPE_POINT:
			totalLight += PointLightPBR(light, input.normal, input.worldPosition, cameraPosition, roughness, metalness, surfaceColor.rgb, specularColor);
			break;
		}
	}

	return float4(pow(totalLight, 1.0f / 2.2f), 1);
}