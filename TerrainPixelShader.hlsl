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

Texture2D GrassTexture : register(t0);
Texture2D CliffTexture : register(t1);
SamplerState BasicSampler : register(s0);

float4 main(VertexToPixel input) : SV_TARGET
{
    input.normal = normalize(input.normal);
    input.tangent = normalize(input.tangent);

    //return float4(input.normal, 1);
    
    input.uv = input.uv * UVScale + UVOffset;
    
    float3 grassColor = GrassTexture.Sample(BasicSampler, input.uv).rgb;
    grassColor.rgb = pow(grassColor.rgb, 2.2f);
    
    float3 cliffColor = CliffTexture.Sample(BasicSampler, input.uv).rgb;
    cliffColor.rgb = pow(cliffColor.rgb, 2.2f);

    float steepness = dot(input.normal, float3(0, 1.0f, 0));
    float3 surfaceColor = lerp(cliffColor, grassColor, steepness);
        
    float3 totalLight = ambient * surfaceColor.rgb;
    
    for (int i = 0; i < NUM_LIGHTS; i++)
    {
        Light light = lights[i];
        light.Direction = normalize(light.Direction);

        switch (light.Type)
        {
            case LIGHT_TYPE_DIRECTIONAL:
                totalLight += DirLight(light, input.normal, input.worldPosition, cameraPosition, roughness, surfaceColor.rgb);
                break;

            case LIGHT_TYPE_POINT:
                totalLight += PointLight(light, input.normal, input.worldPosition, cameraPosition, roughness, surfaceColor.rgb);
                break;
        }
    }

    return float4(pow(totalLight, 1.0f / 2.2f), 1);
}