#include "Light.h"


Light::Light()
	:
	position(XMFLOAT3{ 0.0f,0.0f,0.0f }),
	rotation(XMFLOAT3{ 0.0f,0.0f,0.0f }),
	positionV(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)),
	rotationV(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)),
	colour{ XMVectorSet(0.0f,0.0f,0.0f,1.0f) }
{ }

void Light::Translate(const XMVECTOR _pos)
{
	XMFLOAT3 trans = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMStoreFloat3(&trans, _pos);
	position.x += trans.x;
	position.y += trans.y;
	position.z += trans.z;
}

void Light::Translate(float _x, float _y, float _z)
{
	position.x += _x;
	position.y += _y;
	position.z += _z;
}

void Light::Rotate(const XMVECTOR _rot)
{
	XMFLOAT3 rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMStoreFloat3(&rot, _rot);
	rotation.x += rot.x;
	rotation.y += rot.y;
	rotation.z += rot.z;
}

void Light::Rotate(float _x, float _y, float _z)
{
	rotation.x += _x;
	rotation.y += _y;
	rotation.z += _z;
}

XMFLOAT3 Light::GetPositionFloat3() const
{
	return position;
}

XMVECTOR Light::GetPositionVector()
{
	positionV = DirectX::XMLoadFloat3(&position);
	return positionV;
}

XMFLOAT3 Light::GetRotationFloat3() const
{
	return rotation;
}

XMVECTOR Light::GetRotationVector()
{
	rotationV = XMLoadFloat3(&rotation);
	return rotationV;
}

XMVECTOR Light::GetColour() const
{
	return colour;
}

void Light::SetColour(float _r, float _g, float _b, float _a)
{
	colour = XMVectorSet(_r, _g, _b, _a);
}