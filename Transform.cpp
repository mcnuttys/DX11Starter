#include "Transform.h"

using namespace DirectX;

Transform::Transform()
{
	SetPosition(0, 0, 0);
	SetRotation(0, 0, 0);
	SetScale(1, 1, 1);

	rightVector = XMFLOAT3(1, 0, 0);
	upVector = XMFLOAT3(0, 1, 0);
	forwardVector = XMFLOAT3(0, 0, 1);

	XMStoreFloat4x4(&worldMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&worldInverseTransposeMatrix, XMMatrixIdentity());

	matricesDirty = false;
	vectorsDirty = false;
}

void Transform::MoveAbsolute(float x, float y, float z)
{
	position.x += x;
	position.y += y;
	position.z += z;
	matricesDirty = true;
}

void Transform::MoveRelative(float x, float y, float z)
{
	XMVECTOR move = XMVectorSet(x, y, z, 0);
	XMVECTOR rot = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&pitchYawRoll));

	XMVECTOR dir = XMVector3Rotate(move, rot);

	XMStoreFloat3(&position, XMLoadFloat3(&position) + dir);
	matricesDirty = true;
}

void Transform::Rotate(float p, float y, float r)
{
	pitchYawRoll.x += p;
	pitchYawRoll.y += y;
	pitchYawRoll.z += r;
	matricesDirty = true;
	vectorsDirty = true;
}

void Transform::Scale(float x, float y, float z)
{
	scale.x *= x;
	scale.y *= y;
	scale.z *= z;
	matricesDirty = true;
}

void Transform::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
	matricesDirty = true;
}

void Transform::SetRotation(float p, float y, float r)
{
	pitchYawRoll.x = p;
	pitchYawRoll.y = y;
	pitchYawRoll.z = r;
	matricesDirty = true;
	vectorsDirty = true;
}

void Transform::SetScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
	matricesDirty = true;
}

DirectX::XMFLOAT3 Transform::GetPosition() { return position; }
DirectX::XMFLOAT3 Transform::GetRotation() { return pitchYawRoll; }
DirectX::XMFLOAT3 Transform::GetScale() { return scale; }

DirectX::XMFLOAT3 Transform::GetRight()
{
	UpdateVectors();
	return rightVector;
}

DirectX::XMFLOAT3 Transform::GetUp()
{
	UpdateVectors();
	return upVector;
}

DirectX::XMFLOAT3 Transform::GetForward()
{
	UpdateVectors();
	return forwardVector;
}

DirectX::XMFLOAT4X4 Transform::GetWorldMatrix()
{
	UpdateMatrices();
	return worldMatrix;
}

DirectX::XMFLOAT4X4 Transform::GetWorldInverseTransposeMatrix()
{
	UpdateMatrices();
	return worldInverseTransposeMatrix;
}

void Transform::UpdateMatrices() {
	if (!matricesDirty) return;

	XMMATRIX t = XMMatrixTranslationFromVector(XMLoadFloat3(&position));
	XMMATRIX r = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&pitchYawRoll));
	XMMATRIX s = XMMatrixScalingFromVector(XMLoadFloat3(&scale));

	XMMATRIX world = s * r * t;
	XMStoreFloat4x4(&worldMatrix, world);
	XMStoreFloat4x4(&worldInverseTransposeMatrix, XMMatrixInverse(0, XMMatrixTranspose(world)));

	matricesDirty = false;
}

void Transform::UpdateVectors() {
	if (!vectorsDirty) return;

	XMVECTOR rot = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&pitchYawRoll));

	XMStoreFloat3(&upVector, XMVector3Rotate(XMVectorSet(0, 1, 0, 0), rot));
	XMStoreFloat3(&rightVector, XMVector3Rotate(XMVectorSet(1, 0, 0, 0), rot));
	XMStoreFloat3(&forwardVector, XMVector3Rotate(XMVectorSet(0, 0, 1, 0), rot));

	vectorsDirty = false;
}