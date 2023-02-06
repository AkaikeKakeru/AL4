#pragma once

#include "CollisionTypes.h"
#include "CollisionInfo.h"
#include "Object3d.h"

class BaseCollider {
public:
	BaseCollider() = default;

	//仮想デストラクタ
	virtual ~BaseCollider() = default;

	inline Object3d* GetObject3d() {return object3d_;}
	inline void SetObject(Object3d* object) {object3d_ = object;}

	virtual void Update() = 0;
	inline CollisionShapeType GetShapeType() { return shapeType_; }

	inline void OnCollision(const CollisionInfo& info) {
		object3d_->OnCollision(info);
	}

protected:
	Object3d* object3d_ = nullptr;
	CollisionShapeType shapeType_ = SHAPE_UNKNOWN;
};