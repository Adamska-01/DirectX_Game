#include "GameObject.h"

GameObject::GameObject()
{
    pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
    posVector = XMLoadFloat3(&pos);
    rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
    rotVector = XMLoadFloat3(&rot);
    scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
    scaleVector = XMLoadFloat3(&scale);
}

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

const XMVECTOR& GameObject::GetScaleVector() const
{
    return scaleVector;
}

const XMFLOAT3& GameObject::GetScaleFloat3() const
{
    return scale;
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

void GameObject::CalculateBoundingBoxWorldPos()
{
    XMMATRIX scale = XMMatrixScaling(this->scale.x, this->scale.y, this->scale.z);
    XMMATRIX rot = XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z);
    XMMATRIX pos = XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z);

    XMMATRIX world = scale * rot * pos;

    //min
    box.minBoundV = XMLoadFloat3(&box.minBound);
    box.minBoundV = XMVector3Transform(box.minBoundV, world);
    //max
    box.maxBoundV = XMLoadFloat3(&box.maxBound);
    box.maxBoundV = XMVector3Transform(box.maxBoundV, world);
}

void GameObject::CalculateBoundingSphereWorldPos()
{
    //Set world matrix 
    XMMATRIX scale = XMMatrixScaling(this->scale.x, this->scale.y, this->scale.z);
    XMMATRIX rot = XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z);
    XMMATRIX pos = XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z);

    XMMATRIX world = scale * rot * pos;

    sphere.centreWorldPos = XMLoadFloat3(&sphere.centre);
    sphere.centreWorldPos = XMVector3Transform(sphere.centreWorldPos, world);
}

void GameObject::CalculateBoundingSphereWorldPos(XMVECTOR newPos)
{
    //Set world matrix 
    XMMATRIX scale = XMMatrixScaling(this->scale.x, this->scale.y, this->scale.z);
    XMMATRIX rot = XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z);
    XMMATRIX pos = XMMatrixTranslationFromVector(newPos);

    XMMATRIX world = scale * rot * pos;

    sphere.centreWorldPos = XMLoadFloat3(&sphere.centre);
    sphere.centreWorldPos = XMVector3Transform(sphere.centreWorldPos, world);
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

void GameObject::Scale(const XMVECTOR& _scale)
{
    XMStoreFloat3(&scale, _scale);
    scaleVector = _scale; 
}

void GameObject::Scale(float _x, float _y, float _z)
{
    scale = XMFLOAT3(_x, _y, _z);
    scaleVector = XMLoadFloat3(&scale); 
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
