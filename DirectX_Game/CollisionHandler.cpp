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

const CollisionData& CollisionHandler::SphereToBoxCollision(BoundingSphere a, BoundingBox b)
{
    // Get box closest point to sphere center by clamping
    auto x = std::max(XMVectorGetX(b.minBoundV), std::min(XMVectorGetX(a.centreWorldPos), XMVectorGetX(b.maxBoundV)));
    auto y = std::max(XMVectorGetY(b.minBoundV), std::min(XMVectorGetY(a.centreWorldPos), XMVectorGetY(b.maxBoundV)));
    auto z = std::max(XMVectorGetZ(b.minBoundV), std::min(XMVectorGetZ(a.centreWorldPos), XMVectorGetZ(b.maxBoundV)));

    // Calculate the vector from the center of the sphere to the closest point on the box
    auto closestPoint = XMVectorSet(x, y, z, 0.0f);
    auto sphereToClosestPoint = closestPoint - a.centreWorldPos;

    auto distance = XMVectorGetX(XMVector3Length(sphereToClosestPoint));
    auto penetrationDepth = a.radius - distance; // Calculate penetration depth

    // Calculate collision point on the sphere's surface
    auto collisionPoint = a.centreWorldPos + XMVector3Normalize(sphereToClosestPoint) * a.radius;

    return CollisionData
    {
        distance < a.radius,
        distance,
        penetrationDepth,
        collisionPoint
    };
}
