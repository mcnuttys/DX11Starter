#include "Camera.h"
#include "Input.h"

using namespace DirectX;

Camera::Camera(float aspectRatio, XMFLOAT3 startPosition)
{
    transform.SetPosition(startPosition.x, startPosition.y, startPosition.z);

    fov = XM_PIDIV4;
    nearPlane = 0.1f;
    farPlane = 1000.0f;
    moveSpeed = 3.0f;
    mouseSpeed = 3.0f;

    UpdateViewMatrix();
    UpdateProjectionMatrix(aspectRatio);
}


void Camera::Update(float dt)
{
    Input& input = Input::GetInstance();
    float speed = moveSpeed * dt;

    XMFLOAT3 pos = transform.GetPosition();
    XMFLOAT3 rot = transform.GetRotation();

    if (input.KeyDown('W')) { transform.MoveRelative(0, 0, speed); }
    if (input.KeyDown('S')) { transform.MoveRelative(0, 0, -speed); }
    if (input.KeyDown('A')) { transform.MoveRelative(-speed, 0, 0); }
    if (input.KeyDown('D')) { transform.MoveRelative(speed, 0, 0); }


    if (input.KeyDown('X')) { transform.MoveAbsolute(0, -speed, 0); }
    if (input.KeyDown(VK_SPACE)) { transform.MoveAbsolute(0, speed, 0); }

    if (input.MouseLeftDown())
    {
        float xDiff = mouseSpeed * dt * input.GetMouseXDelta();
        float yDiff = mouseSpeed * dt * input.GetMouseYDelta();
        transform.Rotate(yDiff, xDiff, 0);

        XMFLOAT3 rot = transform.GetRotation();
        if (rot.x > XM_PIDIV2) rot.x = XM_PIDIV2;
        if (rot.x < -XM_PIDIV2) rot.x = -XM_PIDIV2;
        transform.SetRotation(rot.x, rot.y, rot.z);
    }

    UpdateViewMatrix();
}

void Camera::UpdateViewMatrix()
{
    XMFLOAT3 forward = transform.GetForward();
    XMFLOAT3 pos = transform.GetPosition();

    XMMATRIX view = XMMatrixLookToLH(
        XMLoadFloat3(&pos),
        XMLoadFloat3(&forward),
        XMVectorSet(0, 1, 0, 0)
    );
    XMStoreFloat4x4(&viewMatrix, view);
}

void Camera::UpdateProjectionMatrix(float aspectRatio)
{
    XMMATRIX pMatrix = XMMatrixPerspectiveFovLH(fov, aspectRatio, nearPlane, farPlane);
    XMStoreFloat4x4(&projectionMatrix, pMatrix);
}

DirectX::XMFLOAT4X4 Camera::GetViewMatrix()
{
    return viewMatrix;
}

DirectX::XMFLOAT4X4 Camera::GetProjectionMatrix()
{
    return projectionMatrix;
}

Transform* Camera::GetTransform() {
    return &transform;
}
