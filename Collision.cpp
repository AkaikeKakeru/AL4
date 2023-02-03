#include "Collision.h"
#include "MyMath.h"

bool Collision::CheckSphereToPlane(const Sphere& sphere,
	const Plane& plane, Vector3* inter) {
	//座標系の原点から球の中心座標への距離
	float distV = Vector3Dot(sphere.center_, plane.normal_);

	//平面の原点距離を減算することで、平面と球の中心との距離が出る
	float dist = distV - plane.distance_;

	//距離の絶対値が半径より大きければ当たっていない
	if (AbsoluteValue(dist) > sphere.radius_) return false;

	//疑似交点を計算
	if (inter) {
		//平面上の最近接点を、疑似交点とする
		*inter = -dist * plane.normal_ + sphere.center_;
	}

	return true;
}
