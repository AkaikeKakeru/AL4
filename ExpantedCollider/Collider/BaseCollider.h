#pragma once

#include "CollisionTypes.h"
#include "CollisionInfo.h"
#include "Object3d.h"

class BaseCollider {
public:
	friend class CollisionManager;

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

	inline void SetAttribute(unsigned short attribute) {
		attribute_ = attribute;
	}

	inline void AddAttribute(unsigned short attribute) {
		attribute_ |= attribute;
	}

	inline void RemoveAttribute(unsigned short attribute) {
		attribute_ &= !attribute;
	}

protected:
	Object3d* object3d_ = nullptr;
	CollisionShapeType shapeType_ = SHAPE_UNKNOWN;

	//衝突属性
	unsigned short attribute_ = 0b1111111111111111;
};