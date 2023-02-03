#pragma once

#include "CollisionPrimitive.h"

class Collision {
public:
	static bool CheckSphereToPlane(
		const Sphere& sphere, const Plane& plane, Vector3* inter = nullptr);
};