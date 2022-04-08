#ifndef __GGP_SHADER_INCLUDES__
#define __GGP_SHADER_INCLUDES__

// Code stuffs

struct VertexShaderInput
{
	float3 localPosition	: POSITION;     // XYZ position
	float2 uv				: TEXCOORD;
	float3 normal			: NORMAL;
	float3 tangent			: TANGENT;
};

struct VertexToPixel
{
	float4 screenPosition	: SV_POSITION;	// XYZW position (System Value Position)
	float2 uv				: TEXCOORD;
	float3 normal			: NORMAL;
	float3 tangent			: TANGENT;
	float3 worldPosition	: POSITION;
};

struct VertexToPixel_Sky
{
	float4 screenPosition		: SV_POSITION;
	float3 sampleDir	: DIRECTION;
};

#endif