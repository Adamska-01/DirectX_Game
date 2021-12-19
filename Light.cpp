#include "Light.h"


Light::Light() 
{ 
	pos = XMFLOAT3{ 0.0f,0.0f,0.0f };
	rot = XMFLOAT3{ 0.0f,0.0f,0.0f };
	posVector = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	rotVector = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	colour = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
}  

XMVECTOR Light::GetColour() const
{
	return colour;
}

void Light::SetColour(float _r, float _g, float _b, float _a)
{
	colour = XMVectorSet(_r, _g, _b, _a);
}