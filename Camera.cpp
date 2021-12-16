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

const XMVECTOR& Camera::GetPositionVector() const
{
	return posVector;
}

const XMFLOAT3& Camera::GetPositionFloat3() const
{
	return pos;
}

const XMVECTOR& Camera::GetRotationVector() const
{
	return rotVector;
}

const XMFLOAT3& Camera::GetRotationFloat3() const
{
	return rot;
}

const XMVECTOR& Camera::GetForwardVector()
{
	return vec_forward;
}

const XMVECTOR& Camera::GetBackwardVector()
{
	return vec_backward;
}

const XMVECTOR& Camera::GetRightVector()
{
	return vec_right;
}

const XMVECTOR& Camera::GetLeftVector()
{
	return vec_left;
}

void Camera::SetPosition(const XMVECTOR& _pos)
{
	XMStoreFloat3(&pos, _pos);
	posVector = _pos;
	UpdateViewMatrix();
}

void Camera::SetPosition(float _x, float _y, float _z)
{
	pos = XMFLOAT3(_x, _y, _z);
	posVector = XMLoadFloat3(&pos);
	UpdateViewMatrix();
}

void Camera::AdjustPosition(const XMVECTOR& _pos)
{
	posVector += _pos;
	XMStoreFloat3(&pos, posVector);
	UpdateViewMatrix();
}

void Camera::AdjustPosition(float _x, float _y, float _z)
{
	pos.x += _x;
	pos.y += _y;
	pos.z += _z;
	posVector = XMLoadFloat3(&pos);
	UpdateViewMatrix();
}

void Camera::SetRotation(const XMVECTOR& _rot)
{
	rotVector = _rot;
	XMStoreFloat3(&rot, _rot);
	UpdateViewMatrix();
}

void Camera::SetRotation(float _x, float _y, float _z)
{
	rot = XMFLOAT3(_x, _y, _z);
	rotVector = XMLoadFloat3(&rot);
	UpdateViewMatrix();
}

void Camera::AdjustRotation(const XMVECTOR& _rot)
{
	rotVector += _rot;
	XMStoreFloat3(&rot, rotVector);
	UpdateViewMatrix();
}

void Camera::AdjustRotation(float _x, float _y, float _z)
{
	rot.x += _x;
	rot.y += _y;
	rot.z += _z;
	rotVector = XMLoadFloat3(&rot);
	UpdateViewMatrix();
}

void Camera::SetLookAtPos(XMFLOAT3 _lookAtPos)
{
	//Make sure that the look at pos is not the same as the camera pos
	if (_lookAtPos.x == pos.x && _lookAtPos.y == pos.y && _lookAtPos.z == pos.z)
		return;

	//Target - position
	_lookAtPos.x = pos.x - _lookAtPos.x;
	_lookAtPos.y = pos.y - _lookAtPos.y;
	_lookAtPos.z = pos.z - _lookAtPos.z;

	float pitch = 0.0f;
	if (_lookAtPos.y != 0.0f)
	{
		const float distance = sqrt(_lookAtPos.x * _lookAtPos.x + _lookAtPos.z * _lookAtPos.z);
		pitch = atan(_lookAtPos.y / distance);
	}

	float yaw = 0.0f;
	if (_lookAtPos.x != 0.0f)
		yaw = atan(_lookAtPos.x / _lookAtPos.z);
	if (_lookAtPos.z > 0) //Check if whe are on positive z (turn 180 degrees to from a triangle)
		yaw += XM_PI;

	SetRotation(pitch, yaw, 0.0f);
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
