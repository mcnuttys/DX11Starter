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
	material->GetPixelShader()->SetShader();

	std::shared_ptr<SimpleVertexShader> vs = material->GetVertexShader();
	vs->SetShader();

	vs->SetMatrix4x4("world", transform.GetWorldMatrix());
	vs->SetMatrix4x4("view", camera->GetViewMatrix());
	vs->SetMatrix4x4("projection", camera->GetProjectionMatrix());

	vs->CopyAllBufferData();

	std::shared_ptr<SimplePixelShader> ps = material->GetPixelShader();
	ps->SetShader();
	ps->SetFloat4("colorTint", material->GetColorTint());
	ps->SetFloat("time", totalTime);
	ps->SetFloat2("circle0", XMFLOAT2(sin(totalTime), cos(totalTime)));
	ps->SetFloat2("circle1", XMFLOAT2(sin(-totalTime * 5), cos(totalTime * 5)));
	ps->SetFloat2("circle2", XMFLOAT2(sin(totalTime * 0.25), cos(-totalTime * 0.25)));

	ps->CopyAllBufferData();

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
