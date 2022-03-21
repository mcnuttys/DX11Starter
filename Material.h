#pragma once
#include "SimpleShader.h"

#include <DirectXMath.h>
#include <memory>

class Material
{
public:
	Material(DirectX::XMFLOAT3 colorTint, float roughness, std::shared_ptr<SimpleVertexShader> vertexShader, std::shared_ptr<SimplePixelShader> pixelShader);

	DirectX::XMFLOAT3 GetColorTint();
	float GetRoughness();
	std::shared_ptr<SimpleVertexShader> GetVertexShader();
	std::shared_ptr<SimplePixelShader> GetPixelShader();

	void SetColorTint(DirectX::XMFLOAT3 colorTint);
	void SetRoughness(float _roughness);
	void SetVertexShader(std::shared_ptr<SimpleVertexShader> vertexShader);
	void SetPixelShader(std::shared_ptr<SimplePixelShader> pixelShader);

private:
	DirectX::XMFLOAT3 colorTint;
	std::shared_ptr<SimpleVertexShader> vertexShader;
	std::shared_ptr<SimplePixelShader> pixelShader;

	float roughness;
};

