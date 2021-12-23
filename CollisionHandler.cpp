#define NOMINMAX
#include "CollisionHandler.h"
#include <algorithm>

bool CollisionHandler::BoxToBoxCollision(BoundingBox a, BoundingBox b)
{ 
    return (a.minBound.x <= b.maxBound.x && a.maxBound.x >= b.minBound.x) &&
        (a.minBound.y <= b.maxBound.y && a.maxBound.y >= b.minBound.y) &&
        (a.minBound.z <= b.maxBound.z && a.maxBound.z >= b.minBound.z);
}

bool CollisionHandler::SphereToSphereCollision(BoundingSphere a, BoundingSphere b)
{
    return false;
}

bool CollisionHandler::SphereToBoxCollision(BoundingSphere a, BoundingBox b)
{
    // get box closest point to sphere center by clamping
    float x = std::max(XMVectorGetX(b.minBoundV), std::min(XMVectorGetX(a.colSphereCentreWorldPos), XMVectorGetX(b.maxBoundV)));
    float y = std::max(XMVectorGetY(b.minBoundV), std::min(XMVectorGetY(a.colSphereCentreWorldPos), XMVectorGetY(b.maxBoundV)));
    float z = std::max(XMVectorGetZ(b.minBoundV), std::min(XMVectorGetZ(a.colSphereCentreWorldPos), XMVectorGetZ(b.maxBoundV)));
     
    float distance = sqrt((x - XMVectorGetX(a.colSphereCentreWorldPos)) * (x - XMVectorGetX(a.colSphereCentreWorldPos)) +
        (y - XMVectorGetY(a.colSphereCentreWorldPos)) * (y - XMVectorGetY(a.colSphereCentreWorldPos)) +
        (z - XMVectorGetZ(a.colSphereCentreWorldPos)) * (z - XMVectorGetZ(a.colSphereCentreWorldPos)));

    return distance < a.colSphereRadius;
}
