#include "BoundingSphere.h"

void BoundingSphere::CalculateModelCentrePoint(ObjFileModel* _pObj)
{
    XMFLOAT3 minBound = { 0.0f,0.0f,0.0f };
    XMFLOAT3 maxBound = { 0.0f,0.0f,0.0f };
    for (int i = 0; i < _pObj->numverts; i++)
    {
        //X
        if (_pObj->vertices[i].Pos.x > maxBound.x)
            maxBound.x = _pObj->vertices[i].Pos.x;
        if (_pObj->vertices[i].Pos.x < minBound.x)
            minBound.x = _pObj->vertices[i].Pos.x;

        //Y
        if (_pObj->vertices[i].Pos.y > maxBound.y)
            maxBound.y = _pObj->vertices[i].Pos.y;
        if (_pObj->vertices[i].Pos.y < minBound.y)
            minBound.y = _pObj->vertices[i].Pos.y;

        //Z
        if (_pObj->vertices[i].Pos.z > maxBound.z)
            maxBound.z = _pObj->vertices[i].Pos.z;
        if (_pObj->vertices[i].Pos.z < minBound.z)
            minBound.z = _pObj->vertices[i].Pos.z;
    }

    //Calculate model centre
    XMFLOAT3 maxMinusmin = { (maxBound.x - minBound.x) / 2, (maxBound.y - minBound.y) / 2, (maxBound.z - minBound.z) / 2 };
    XMFLOAT3 minPlusMaxMinusMin = { minBound.x + maxMinusmin.x,  minBound.y + maxMinusmin.y, minBound.z + maxMinusmin.z };

    centre = minPlusMaxMinusMin;
}

void BoundingSphere::CalculateBoundingSphereRadius(ObjFileModel* _pObj, float _scale)
{
    for (int i = 0; i < _pObj->numverts; i++)
    {
        float rad = sqrt(pow((centre.x - _pObj->vertices[i].Pos.x), 2) + pow((centre.y - _pObj->vertices[i].Pos.y), 2) + pow((centre.z - _pObj->vertices[i].Pos.z), 2));

        if (radius < rad)
            radius = rad;
    }

    radius *= _scale;
}
