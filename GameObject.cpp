#include "GameObject.h"

const XMVECTOR& GameObject::GetPositionVector() const
{
    return posVector;
}

const XMFLOAT3& GameObject::GetPositionFloat3() const
{
    return pos;
}

const XMVECTOR& GameObject::GetRotationVector() const
{
    return rotVector;
}

const XMFLOAT3& GameObject::GetRotationFloat3() const
{
    return rot;
}

const XMVECTOR& GameObject::GetForwardVector()
{
    return vec_forward;
}

const XMVECTOR& GameObject::GetBackwardVector()
{
    return vec_backward;
}

const XMVECTOR& GameObject::GetRightVector()
{
    return vec_right;
}

const XMVECTOR& GameObject::GetLeftVector()
{
    return vec_left;
}

void GameObject::SetPosition(const XMVECTOR& _pos)
{
    XMStoreFloat3(&pos, _pos);
    posVector = _pos; 
    UpdateVectors();
}

void GameObject::SetPosition(float _x, float _y, float _z)
{
    pos = XMFLOAT3(_x, _y, _z);
    posVector = XMLoadFloat3(&pos);
    UpdateVectors();
}

void GameObject::AdjustPosition(const XMVECTOR& _pos)
{
    posVector += _pos;
    XMStoreFloat3(&pos, posVector);
    UpdateVectors();
}

void GameObject::AdjustPosition(float _x, float _y, float _z)
{
    pos.x += _x;
    pos.y += _y;
    pos.z += _z;
    posVector = XMLoadFloat3(&pos);
    UpdateVectors();
}

void GameObject::SetRotation(const XMVECTOR& _rot)
{
    rotVector = _rot;
    XMStoreFloat3(&rot, _rot);
    UpdateVectors();
}

void GameObject::SetRotation(float _x, float _y, float _z)
{
    rot = XMFLOAT3(_x, _y, _z);
    rotVector = XMLoadFloat3(&rot);
    UpdateVectors();
}

void GameObject::AdjustRotation(const XMVECTOR& _rot)
{
    rotVector += _rot;
    XMStoreFloat3(&rot, rotVector);
    UpdateVectors();
}

void GameObject::AdjustRotation(float _x, float _y, float _z)
{
    rot.x += _x;
    rot.y += _y;
    rot.z += _z;
    rotVector = XMLoadFloat3(&rot);
    UpdateVectors();
}

void GameObject::SetLookAtPos(XMFLOAT3 _lookAtPos)
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

void GameObject::UpdateVectors()
{
    //Update vectors (pitch 0.0f, no y movement)
    XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, rot.y, 0.0f);
    vec_forward = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
    vec_backward = XMVector3TransformCoord(DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
    vec_left = XMVector3TransformCoord(DEFAULT_LEFT_VECTOR, vecRotationMatrix);
    vec_right = XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
}
