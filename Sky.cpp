#include "Sky.h"

Sky::Sky(
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cubeMap,
	Mesh* mesh,
	std::shared_ptr<SimpleVertexShader> _skyVS,
	std::shared_ptr<SimplePixelShader> _skyPS,
	Microsoft::WRL::ComPtr<ID3D11SamplerState> _sampler,
	Microsoft::WRL::ComPtr<ID3D11Device> _device,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> _context
)
{
	skySRV = cubeMap;
	skyMesh = mesh;
	skyVS = _skyVS;
	skyPS = _skyPS;
	sampler = _sampler;
	device = _device;
	context = _context;


	D3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_FRONT;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.DepthClipEnable = true;
	device->CreateRasterizerState(&rastDesc, skyRasterState.GetAddressOf());

	D3D11_DEPTH_STENCIL_DESC depthDesc = {};
	depthDesc.DepthEnable = true;
	depthDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	device->CreateDepthStencilState(&depthDesc, skyDepthState.GetAddressOf());
}

void Sky::Draw(Camera* camera)
{
	context->RSSetState(skyRasterState.Get());
	context->OMSetDepthStencilState(skyDepthState.Get(), 0);

	skyVS->SetShader();
	skyPS->SetShader();

	skyVS->SetMatrix4x4("view", camera->GetViewMatrix());
	skyVS->SetMatrix4x4("projection", camera->GetProjectionMatrix());
	skyVS->CopyAllBufferData();

	skyPS->SetShaderResourceView("SkyTexture", skySRV);
	skyPS->SetSamplerState("BasicSampler", sampler);
	skyPS->CopyAllBufferData();

	skyMesh->Draw();

	context->RSSetState(0);
	context->OMSetDepthStencilState(0, 0);
}
