#pragma once
#include "Transform.h"
#include "Camera.h"
#include "Mesh.h"
#include "Material.h"

#include <memory>

class GameEntity
{
public:
	GameEntity(Mesh* _mesh, std::shared_ptr<Material> material);

	Transform* GetTransform();
	Mesh* GetMesh();

	void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, std::shared_ptr<Camera> camera, float totalTime);

	std::shared_ptr<Material> GetMaterial();
	void SetMaterial(std::shared_ptr<Material> _material);

private:
	Transform transform;
	Mesh* mesh;
	std::shared_ptr<Material> material;
};

