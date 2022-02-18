#include "GameEntity.h"
#include "Camera.h"
#include "BufferStructs.h"

#include <memory>

using namespace DirectX;

GameEntity::GameEntity(Mesh* _mesh)
{
    mesh = _mesh;
}

Transform* GameEntity::GetTransform() { return &transform; }

Mesh* GameEntity::GetMesh() { return mesh; }

void GameEntity::Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Microsoft::WRL::ComPtr<ID3D11Buffer> vsConstantBuffer, std::shared_ptr<Camera> camera)
{
	VertexShaderExternalData vsData;
	vsData.worldMatrix = transform.GetWorldMatrix();
	vsData.viewMatrix = camera->GetViewMatrix();
	vsData.projectionMatrix = camera->GetProjectionMatrix();

	D3D11_MAPPED_SUBRESOURCE mappedBuffer = {};
	context->Map(vsConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);

	memcpy(mappedBuffer.pData, &vsData, sizeof(vsData));
	context->Unmap(vsConstantBuffer.Get(), 0);

	mesh->Draw();
}
