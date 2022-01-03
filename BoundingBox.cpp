#include "BoundingBox.h"

void BoundingBox::CalculateMinAndMax(ObjFileModel* _pObj)
{
    for (unsigned int i = 0; i < _pObj->numverts; i++)
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
}
