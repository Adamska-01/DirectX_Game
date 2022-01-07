#pragma once
#include "Light.h"


class DirectionalLight : public Light
{
private:
	XMVECTOR direction;  

public:
	DirectionalLight();

	XMVECTOR GetDirection(XMMATRIX _world);

	void SetDirection(float _x, float _y, float _z);
};

