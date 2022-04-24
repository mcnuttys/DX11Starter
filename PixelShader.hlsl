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
	input.normal = normalize(input.normal);
	input.tangent = normalize(input.tangent);

	input.uv = input.uv * UVScale + UVOffset;

	float3 unpackedNormal = NormalMap.Sample(BasicSampler, input.uv).rgb * 2.0f - 1.0f;
	
	float3 N = input.normal; 
	float3 T = input.tangent;
	T = normalize(T - N * dot(T, N));
    float3 B = cross(T, N);
    float3x3 TBN = float3x3(T, B, N);
    input.normal = normalize(mul(unpackedNormal, TBN));
	
    float roughness = RoughnessMap.Sample(BasicSampler, input.uv).r;
    float metalness = MetalnessMap.Sample(BasicSampler, input.uv).r;

	// Specular color determination -----------------
	// Assume albedo texture is actually holding specular color where metalness == 1
	//
	// Note the use of lerp here - metal is generally 0 or 1, but might be in between
	// because of linear texture sampling, so we lerp the specular color to match

	float3 surfaceColor = Albedo.Sample(BasicSampler, input.uv).rgb;
	surfaceColor.rgb = pow(surfaceColor.rgb, 2.2f);

	float3 specularColor = lerp(F0_NON_METAL.rrr, surfaceColor.rgb, metalness);

    float3 totalLight = ambient * surfaceColor.rgb;

	for (int i = 0; i < NUM_LIGHTS; i++) {
		Light light = lights[i];
		light.Direction = normalize(light.Direction);

		switch (light.Type) {
		case LIGHT_TYPE_DIRECTIONAL:
			totalLight += DirLightPBR(light, input.normal, input.worldPosition, cameraPosition, roughness, metalness, surfaceColor.rgb, specularColor);
			//totalLight += DirLight(light, input.normal, input.worldPosition, cameraPosition, roughness, surfaceColor.rgb);
			break;

		case LIGHT_TYPE_POINT:
			totalLight += PointLightPBR(light, input.normal, input.worldPosition, cameraPosition, roughness, metalness, surfaceColor.rgb, specularColor);
            //totalLight += PointLight(light, input.normal, input.worldPosition, cameraPosition, roughness, surfaceColor.rgb);
			break;
		}
	}

	return float4(pow(totalLight, 1.0f / 2.2f), 1);
}