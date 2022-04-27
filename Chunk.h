#pragma once
#include "Mesh.h"
#include "GameEntity.h"
#include "Node.h"
#include "PerlinNoise.h"

#include <DirectXMath.h>
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects

class Chunk
{
public:
	Chunk(DirectX::XMFLOAT3 position, DirectX::XMINT2 size, Microsoft::WRL::ComPtr<ID3D11Device> _device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext, std::shared_ptr<Material> material);
	Chunk(DirectX::XMFLOAT3 position, DirectX::XMINT3 size, Microsoft::WRL::ComPtr<ID3D11Device> _device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext, std::shared_ptr<Material> material);
	~Chunk();

	GameEntity* GetGameEntity();
	bool emptyChunk;

private:
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;

	DirectX::XMFLOAT3 position;
	DirectX::XMINT3 size;

	Mesh* chunkMesh;
	GameEntity* chunkEntity;

	void GenerateHeightMesh();
	void GenerateMarchingCubesMesh();

	int CubeIndex(Node n0, Node n1, Node n2, Node n3, Node n4, Node n5, Node n6, Node n7);
	DirectX::XMFLOAT3 GetAntiNode(int index, Node n0, Node n1, Node n2, Node n3, Node n4, Node n5, Node n6, Node n7);
	DirectX::XMFLOAT3 Interpolate(Node n0, Node n1);
	int Index3Dto1D(int x, int y, int z);

	DirectX::XMFLOAT2* uvTable;
};

