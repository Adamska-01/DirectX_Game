#include "PointLight.h"

PointLight::PointLight()
	: 
	range(10.0f),
	attenuation(XMFLOAT3(0.0f, 1.0f, 0.0f)) //constant falloff factor
{ }

float PointLight::GetRange()
{
	return range;
}

const XMFLOAT3 PointLight::GetAttenuation()
{
	return attenuation;
}

const XMVECTOR PointLight::GetInversePos(XMMATRIX _world)
{
	XMVECTOR determinant;
	XMMATRIX inverse = XMMatrixInverse(&determinant, _world);

	return XMVector3Transform(XMLoadFloat3(&pos), inverse);
}

void PointLight::SetRange(float _value)
{
	range = _value;
}

void PointLight::SetAttenuation(const XMFLOAT3 _value)
{
	attenuation = _value;
}