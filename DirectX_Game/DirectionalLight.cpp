#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
	:
	direction(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f))
{ }

XMVECTOR DirectionalLight::GetDirection(XMMATRIX _world)
{
	XMMATRIX transpose = XMMatrixTranspose(_world); 
	XMMATRIX rotateLight = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
	XMVECTOR newDir = XMVector3Normalize(XMVector3Transform(XMVector3Transform(direction, rotateLight), transpose));

	return newDir;
}

void DirectionalLight::SetDirection(float _x, float _y, float _z)
{ 
	direction = XMVectorSet(_x, _y, _z, 1.0f);
}
