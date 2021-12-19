#include "Camera.h"

Camera::Camera()
{
	pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	posVector = XMLoadFloat3(&pos);
	rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotVector = XMLoadFloat3(&rot);

	UpdateViewMatrix();
}

void Camera::SetProjectionValues(float _fovDegrees, float _aspectRatio, float _nearZ, float _farZ)
{
	float fovRadians = XMConvertToRadians(_fovDegrees);
	projectionMatrix = XMMatrixPerspectiveFovLH(fovRadians, _aspectRatio, _nearZ, _farZ);
}

const XMMATRIX& Camera::GetViewMatrix() const
{
	return viewMatrix;
}

const XMMATRIX& Camera::GetProjetionMatrix() const
{
	return projectionMatrix;
}
  
void Camera::SetPosition(const XMVECTOR& _pos)
{
	GameObject::SetPosition(_pos);
	UpdateViewMatrix();
}

void Camera::SetPosition(float _x, float _y, float _z)
{
	GameObject::SetPosition(_x, _y, _z);
	UpdateViewMatrix();
}

void Camera::AdjustPosition(const XMVECTOR& _pos)
{
	GameObject::AdjustPosition(_pos);
	UpdateViewMatrix();
}

void Camera::AdjustPosition(float _x, float _y, float _z)
{
	GameObject::AdjustPosition(_x, _y, _z);
	UpdateViewMatrix();
}

void Camera::SetRotation(const XMVECTOR& _rot)
{
	GameObject::SetRotation(_rot);
	UpdateViewMatrix();
}

void Camera::SetRotation(float _x, float _y, float _z)
{
	GameObject::SetRotation(_x, _y, _z);
	UpdateViewMatrix();
}

void Camera::AdjustRotation(const XMVECTOR& _rot)
{
	GameObject::AdjustRotation(_rot);
	UpdateViewMatrix();
}

void Camera::AdjustRotation(float _x, float _y, float _z)
{
	GameObject::AdjustRotation(_x, _y, _z);
	UpdateViewMatrix();
}

void Camera::SetLookAtPos(XMFLOAT3 _lookAtPos)
{
	GameObject::SetLookAtPos(_lookAtPos);
	UpdateViewMatrix();
}

void Camera::UpdateViewMatrix()
{
	//Calculate camera rotation matrix
	XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
	//Calculate unit vector of cam target based off camera forward value transformed by cam rotation
	XMVECTOR camTarget = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, camRotationMatrix);
	//Adjust cam target to be offset by the camera's current position;
	camTarget += posVector;
	//Calculate up direction based on current rotation
	XMVECTOR upDir = XMVector3TransformCoord(DEFAULT_UP_VECTOR, camRotationMatrix);
	
	//Rebuild view matrix
	viewMatrix = XMMatrixLookAtLH(posVector, camTarget, upDir);

	//Update vectors (pitch 0.0f, no y movement)
	XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, rot.y, 0.0f);
	vec_forward = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	vec_backward = XMVector3TransformCoord(DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
	vec_left = XMVector3TransformCoord(DEFAULT_LEFT_VECTOR, vecRotationMatrix);
	vec_right = XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
}
