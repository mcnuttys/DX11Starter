#pragma once
#include "Transform.h"
#include "Camera.h"
#include "Mesh.h"

#include <memory>

class GameEntity
{
public:
	GameEntity(Mesh* _mesh);

	Transform* GetTransform();
	Mesh* GetMesh();

	void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Microsoft::WRL::ComPtr<ID3D11Buffer> vsConstantBuffer, std::shared_ptr<Camera> camera);

private:
	Transform transform;
	Mesh* mesh;
};

