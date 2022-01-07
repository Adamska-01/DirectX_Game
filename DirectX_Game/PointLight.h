#pragma once
#include "Light.h"


class PointLight : public Light
{
private:
	float range;
	XMFLOAT3 attenuation;

public:
	PointLight();

	//Getters
	float GetRange();
	const XMFLOAT3 GetAttenuation();
	const XMVECTOR GetInversePos(XMMATRIX _world);

	//Setters
	void SetRange(float _value);
	void SetAttenuation(const XMFLOAT3 _value);
};

