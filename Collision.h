#pragma once

#include "CollisionPrimitive.h"

class Collision {
public:
	//球と平面の交差判定
	static bool CheckSphereToPlane(
		const Sphere& sphere, const Plane& plane, Vector3* inter = nullptr);
};