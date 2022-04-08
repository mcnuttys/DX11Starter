#pragma once

#include "Mesh.h"
#include "SimpleShader.h"
#include "Camera.h"

#include <memory>
#include <wrl/client.h>

class Sky
{
public:
	Sky(
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cubeMap, 
		Mesh* mesh,
		std::shared_ptr<SimpleVertexShader> skyVS,
		std::shared_ptr<SimplePixelShader> skyPS, 
		Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler,
		Microsoft::WRL::ComPtr<ID3D11Device> device,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context
	);

	void Draw(Camera* camera);

private:
	Mesh* skyMesh;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> skySRV;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> skyDepthState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> skyRasterState;

	std::shared_ptr<SimpleVertexShader> skyVS;
	std::shared_ptr<SimplePixelShader> skyPS;

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	Microsoft::WRL::ComPtr<ID3D11Device> device;
};

