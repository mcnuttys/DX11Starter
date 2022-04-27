#pragma once
#include "Mesh.h"
#include "GameEntity.h"

#include <DirectXMath.h>
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects

class Chunk
{
public:
	Chunk(DirectX::XMFLOAT3 position, DirectX::XMINT3 size, Microsoft::WRL::ComPtr<ID3D11Device> _device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext, std::shared_ptr<Material> material);
	~Chunk();

	GameEntity* GetGameEntity();

private:
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;

	DirectX::XMFLOAT3 position;
	DirectX::XMINT3 size;

	Mesh* chunkMesh;
	GameEntity* chunkEntity;

	void GenerateMesh();
};

