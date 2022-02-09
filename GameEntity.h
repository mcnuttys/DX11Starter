#pragma once
#include "Transform.h"
#include "Mesh.h"

class GameEntity
{
public:
	GameEntity(Mesh* _mesh);

	Transform* GetTransform();
	Mesh* GetMesh();

	void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Microsoft::WRL::ComPtr<ID3D11Buffer> vsConstantBuffer);

private:
	Transform transform;
	Mesh* mesh;
};

