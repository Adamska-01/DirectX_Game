#pragma once
#include "objfilemodel.h" 


class BoundingSphere
{
public: 
	XMVECTOR centreWorldPos;
	XMFLOAT3 centre;
	float radius;

public:
	void CalculateModelCentrePoint(ObjFileModel* _pObj);
	void CalculateBoundingSphereRadius(ObjFileModel* _pObj);
};