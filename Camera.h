#pragma once
#include "Input.h"
#include "Transform.h"

class Camera
{
public:
	Camera(float aspectRatio, DirectX::XMFLOAT3 startPosition);

	void Update(float dt);

	void UpdateViewMatrix();
	void UpdateProjectionMatrix(float aspectRatio);

	DirectX::XMFLOAT4X4 GetViewMatrix();
	DirectX::XMFLOAT4X4 GetProjectionMatrix();

private:
	Transform transform;

	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

	float fov;
	float nearPlane;
	float farPlane;
	float moveSpeed;
	float mouseSpeed;
};