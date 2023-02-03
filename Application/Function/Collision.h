#pragma once

#include "CollisionPrimitive.h"

class Collision {
public:
	//球と球の衝突判定
	bool Collision_SphereToSphere(
		Vector3 posA,Vector3 posB,float radA,float radB);

	//球と平面の交差判定
	static bool CheckSphereToPlane(
		const Sphere& sphere, const Plane& plane, Vector3* inter = nullptr);
};