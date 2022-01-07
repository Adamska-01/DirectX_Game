#pragma once
#include "BoundingBox.h"
#include "BoundingSphere.h"


class CollisionHandler
{ 
public: 
	static bool BoxToBoxCollision(BoundingBox a, BoundingBox b); 
	static bool SphereToSphereCollision(BoundingSphere a, BoundingSphere b);
	static bool SphereToBoxCollision(BoundingSphere a, BoundingBox b);
};