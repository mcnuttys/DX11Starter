#include "ShaderIncludes.hlsli"

cbuffer ExternalData : register(b0)
{
	matrix world; 
	matrix worldInvTranspose;
	matrix view; 
	matrix projection;
}

VertexToPixel main( VertexShaderInput input )
{
	VertexToPixel output;

	matrix mvp = mul(projection, mul(view, world));
	output.screenPosition = mul(mvp, float4(input.localPosition, 1.0f));

	output.uv = input.uv;
	output.normal = normalize(mul((float3x3)worldInvTranspose, input.normal));
	output.worldPosition = mul(world, float4(input.localPosition, 1)).xyz;

	return output;
}