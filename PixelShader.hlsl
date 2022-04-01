#include "ShaderIncludes.hlsli"

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

Texture2D SurfaceTexture : register(t0); // "t" registers for textures
SamplerState BasicSampler : register(s0); // "s" registers for samplers

float4 main(VertexToPixel input) : SV_TARGET
{ 
	input.uv = input.uv * UVScale + UVOffset;

	input.normal = normalize(input.normal);

	float3 surfaceColor = SurfaceTexture.Sample(BasicSampler, input.uv).rgb;
	surfaceColor *= colorTint;

	float3 totalLight = ambient * surfaceColor;

	for (int i = 0; i < NUM_LIGHTS; i++) {
		Light light = lights[i];
		light.Direction = normalize(light.Direction);

		switch (light.Type) {
		case LIGHT_TYPE_DIRECTIONAL:
			totalLight += DirLight(light, input.normal, input.worldPosition, cameraPosition, roughness, colorTint);
			break;

		case LIGHT_TYPE_POINT:
			totalLight += PointLight(light, input.normal, input.worldPosition, cameraPosition, roughness, colorTint);
			break;
		}
	}

	return float4(totalLight, 1);
}