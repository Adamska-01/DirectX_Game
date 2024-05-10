#pragma once


struct CollisionData
{
    bool isColliding;
    float distance;
    float penetrationDepth;
    XMVECTOR collisionPoint;
};