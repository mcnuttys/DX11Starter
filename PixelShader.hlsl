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

Texture2D SurfaceTexture	: register(t0); // "t" registers for textures
Texture2D NormalMap			: register(t2);
SamplerState BasicSampler	: register(s0); // "s" registers for samplers

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