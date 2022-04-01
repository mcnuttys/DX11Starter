#include "GameEntity.h"
#include "Camera.h"
#include <math.h>
#include <memory>

using namespace DirectX;

GameEntity::GameEntity(Mesh* _mesh, std::shared_ptr<Material> material)
{
    mesh = _mesh;

	SetMaterial(material);
}

Transform* GameEntity::GetTransform() { return &transform; }

Mesh* GameEntity::GetMesh() { return mesh; }

void GameEntity::Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, std::shared_ptr<Camera> camera, float totalTime)
{
	material->PrepareMaterial(&transform, camera.get());

	mesh->Draw();
}

std::shared_ptr<Material> GameEntity::GetMaterial()
{
	return material;
}

void GameEntity::SetMaterial(std::shared_ptr<Material> _material)
{
	material = _material;
}
