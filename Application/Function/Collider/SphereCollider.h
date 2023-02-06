#pragma once

#include "BaseCollider.h"
#include "CollisionPrimitive.h"

class SphereCollider : public BaseCollider, public Sphere {
public:
	SphereCollider(Vector3 offset = { 0,0,0 }, float radius = 1.0f) :
		offset_(offset),
		radius_(radius) {
		//球形状をセット
		shapeType_ = COLLISIONSHAPE_SPHERE;
	}

	void Update() override;

	inline void SetRadius(float radius) { radius_ = radius; }

private:
	//オブジェクト中心からのオフセット
	Vector3 offset_;
	//半径
	float radius_;
};