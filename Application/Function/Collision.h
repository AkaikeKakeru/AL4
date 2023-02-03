#pragma once

#include "CollisionPrimitive.h"

class Collision {
public:
	//球と球の衝突判定
	static bool CheckSphere2Sphere(
		Vector3 posA,Vector3 posB,float radA,float radB);

	//球と平面の交差判定
	static bool CheckSphere2Plane(
		const Sphere& sphere, const Plane& plane, Vector3* inter = nullptr);

	//点と三角形の最近接点を求める
	static void ClosessPtPoint2Triangle(
		const Vector3& point, const Triangle& triangle, Vector3* closest);

};