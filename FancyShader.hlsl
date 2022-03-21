#include "ShaderIncludes.hlsli"

cbuffer ExternalData : register(b0)
{
	float4 colorTint;
	float time;
	float2 circle0;
	float2 circle1;
	float2 circle2;
}

float4 main(VertexToPixel input) : SV_TARGET
{
	float2 pos = input.uv;
	float d0 = distance(circle0, pos);
	float d1 = distance(circle1, pos);
	float d2 = distance(circle2, pos);

	float4 color = 
		float4(1, 0, 0, 1) * (d0 < 0.5) +
		float4(0, 1, 0, 1) * (d1 < 0.25) +
		float4(0, 0, 1, 1) * (d2 < 0.75);

	return color;
}