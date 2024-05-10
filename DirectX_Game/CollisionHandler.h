#pragma once
#include "BoundingBox.h"
#include "CollisionData.h"
#include "BoundingSphere.h"


class CollisionHandler
{ 
public: 
	static bool BoxToBoxCollision(BoundingBox a, BoundingBox b); 
	static bool SphereToSphereCollision(BoundingSphere a, BoundingSphere b);
	static const CollisionData& SphereToBoxCollision(BoundingSphere a, BoundingBox b);
};