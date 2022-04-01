#pragma once
#include "SimpleShader.h"
#include "Camera.h"

#include <DirectXMath.h>
#include <memory>

class Material
{
public:
	Material(
		DirectX::XMFLOAT3 colorTint,
		float roughness,
		DirectX::XMFLOAT2 scale,
		DirectX::XMFLOAT2 offset,
		std::shared_ptr<SimpleVertexShader> vertexShader,
		std::shared_ptr<SimplePixelShader> pixelShader
	);

	DirectX::XMFLOAT3 GetColorTint();
	float GetRoughness();
	DirectX::XMFLOAT2 GetUVScale();
	DirectX::XMFLOAT2 GetUVOffset();
	std::shared_ptr<SimpleVertexShader> GetVertexShader();
	std::shared_ptr<SimplePixelShader> GetPixelShader();

	void SetColorTint(DirectX::XMFLOAT3 colorTint);
	void SetRoughness(float _roughness);
	void SetUVScale(DirectX::XMFLOAT2 _scale);
	void SetUVOffset(DirectX::XMFLOAT2 _offset);
	void SetVertexShader(std::shared_ptr<SimpleVertexShader> vertexShader);
	void SetPixelShader(std::shared_ptr<SimplePixelShader> pixelShader);

	void AddTextureSRV(std::string name, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv);
	void AddSampler(std::string name, Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler);

	void PrepareMaterial(Transform* transform, Camera* camera);

private:
	DirectX::XMFLOAT3 colorTint;
	std::shared_ptr<SimpleVertexShader> vertexShader;
	std::shared_ptr<SimplePixelShader> pixelShader;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureResource;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;

	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textureSRVs;
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11SamplerState>> samplers;

	float roughness;
	DirectX::XMFLOAT2 scale;
	DirectX::XMFLOAT2 offset;
};

