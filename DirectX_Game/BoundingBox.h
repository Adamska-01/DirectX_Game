#pragma once
#include "objfilemodel.h" 


class BoundingBox
{
public:
	XMVECTOR minBoundV = { 0.0f,0.0f,0.0f };
	XMVECTOR maxBoundV = { 0.0f,0.0f,0.0f };
	XMFLOAT3 minBound = { 0.0f,0.0f,0.0f };
	XMFLOAT3 maxBound = { 0.0f,0.0f,0.0f };

public:
	void CalculateMinAndMax(ObjFileModel* _pObj);
};