#include "CollisionHandler.h"

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
