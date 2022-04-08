#ifndef __GGP_LIGHTING__
#define __GGP_LIGHTING__

#define LIGHT_TYPE_DIRECTIONAL 0
#define LIGHT_TYPE_POINT 1
#define LIGHT_TYPE_SPOT 2

struct Light {
	int Type;
	float3 Direction;
	float Range;
	float3 Position;
	float Intensity;
	float3 Color;
	float SpotFalloff;
	float3 Padding;
};

#define MAX_SPECULAR_EXPONENT 256.0f

float Diffuse(float3 normal, float3 dirToLight) {
	return saturate(dot(normal, dirToLight));
};

float SpecularPhong(float3 normal, float3 dirToLight, float3 toCamera, float roughness)
{
	float3 refl = reflect(-dirToLight, normal);

	return roughness == 1 ? 0.0f : pow(max(dot(toCamera, refl), 0), (1 - roughness) * MAX_SPECULAR_EXPONENT);
}

float Attenuate(Light light, float3 worldPos)
{
	float dist = distance(light.Position, worldPos);
	float att = saturate(1.0f - (dist * dist / (light.Range * light.Range)));
	return att * att;
}

float3 DirLight(Light light, float3 normal, float3 worldPos, float3 cameraPosition, float roughness, float3 colorTint)
{
	float3 toLight = normalize(-light.Direction);
	float3 toCam = normalize(cameraPosition - worldPos);

	float diffuse = Diffuse(normal, toLight);
	float spec = SpecularPhong(normal, toLight, toCam, roughness);

	return (diffuse * colorTint + spec) * light.Intensity * light.Color;
}

float3 PointLight(Light light, float3 normal, float3 worldPos, float3 camPos, float roughness, float3 surfaceColor)
{
	float3 toLight = normalize(light.Position - worldPos);
	float3 toCam = normalize(camPos - worldPos);

	float attenuate = Attenuate(light, worldPos);
	float diff = Diffuse(normal, toLight);
	float spec = SpecularPhong(normal, toLight, toCam, roughness);

	return (diff * surfaceColor + spec) * attenuate * light.Intensity * light.Color;
}

#endif