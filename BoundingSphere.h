#pragma once
#include "objfilemodel.h" 


class BoundingSphere
{
public: 
	XMFLOAT3 colSphereCentre;
	float colSphereRadius;

public:
	void CalculateModelCentrePoint(ObjFileModel* _pObj);
	void CalculateBoundingSphereRadius(ObjFileModel* _pObj);
};