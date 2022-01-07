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
    float distance = (float)sqrt((XMVectorGetX(a.centreWorldPos) - XMVectorGetX(b.centreWorldPos)) * (XMVectorGetX(a.centreWorldPos) - XMVectorGetX(b.centreWorldPos)) +
        (XMVectorGetY(a.centreWorldPos) - XMVectorGetY(b.centreWorldPos)) * (XMVectorGetY(a.centreWorldPos) - XMVectorGetY(b.centreWorldPos)) +
        (XMVectorGetZ(a.centreWorldPos) - XMVectorGetZ(b.centreWorldPos)) * (XMVectorGetZ(a.centreWorldPos) - XMVectorGetZ(b.centreWorldPos)));
    
    return distance < (a.radius + b.radius);
}

bool CollisionHandler::SphereToBoxCollision(BoundingSphere a, BoundingBox b)
{
    // get box closest point to sphere center by clamping
    float x = std::max(XMVectorGetX(b.minBoundV), std::min(XMVectorGetX(a.centreWorldPos), XMVectorGetX(b.maxBoundV)));
    float y = std::max(XMVectorGetY(b.minBoundV), std::min(XMVectorGetY(a.centreWorldPos), XMVectorGetY(b.maxBoundV)));
    float z = std::max(XMVectorGetZ(b.minBoundV), std::min(XMVectorGetZ(a.centreWorldPos), XMVectorGetZ(b.maxBoundV)));
     
    float distance = (float)sqrt((x - XMVectorGetX(a.centreWorldPos)) * (x - XMVectorGetX(a.centreWorldPos)) +
        (y - XMVectorGetY(a.centreWorldPos)) * (y - XMVectorGetY(a.centreWorldPos)) +
        (z - XMVectorGetZ(a.centreWorldPos)) * (z - XMVectorGetZ(a.centreWorldPos)));

    return distance < a.radius;
}
