#include "GameEntity.h"
#include "BufferStructs.h"

using namespace DirectX;

GameEntity::GameEntity(Mesh* _mesh)
{
    mesh = _mesh;
}

Transform* GameEntity::GetTransform() { return &transform; }

Mesh* GameEntity::GetMesh() { return mesh; }

void GameEntity::Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Microsoft::WRL::ComPtr<ID3D11Buffer> vsConstantBuffer)
{
	VertexShaderExternalData vsData;
	vsData.colorTint = XMFLOAT4(1.0f, 0.5f, 0.5f, 1.0f);
	vsData.worldMatrix = transform.GetWorldMatrix();

	D3D11_MAPPED_SUBRESOURCE mappedBuffer = {};
	context->Map(vsConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);

	memcpy(mappedBuffer.pData, &vsData, sizeof(vsData));
	context->Unmap(vsConstantBuffer.Get(), 0);

	mesh->Draw();
}
