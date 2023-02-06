#pragma once

#include "Object3d.h"

class Character :
	public Object3d {
public:
	//オブジェクト生成
	static Character* Create(Model* model = nullptr);
public:
	//初期化
	bool Initialize() override;

	//更新
	void Update() override;

	//衝突時のコールバック
	void OnCollision(const CollisionInfo& info) override;
};