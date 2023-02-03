#pragma once
#include "Vector3.h"

struct Sphere {
	//中心
	Vector3 center_ = { 0,0,0 };

	//半径
	float radius_ = 1.0f;
};

struct Plane {
	//法線
	Vector3 normal_ = { 0,1,0 };

	//原点からの距離
	float distance_ = 0.0f;
};