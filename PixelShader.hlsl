#include "ShaderIncludes.hlsli"

#define NUM_LIGHTS 5

cbuffer ExternalData : register(b0)
{
	float roughness;
	float3 colorTint;
	float3 cameraPosition; 
	float3 ambient;

	Light lights[NUM_LIGHTS];
}

float4 main(VertexToPixel input) : SV_TARGET
{
	input.normal = normalize(input.normal);

	float3 totalLight = ambient * colorTint;

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