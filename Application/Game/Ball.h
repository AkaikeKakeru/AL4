#pragma once

#include "Object3d.h"
#include "DebugText.h"

class Ball :
	public Object3d {
public:
	//オブジェクト生成
	static Ball* Create(Model* model = nullptr);
public:
	//初期化
	bool Initialize() override;

	//更新
	void Update() override;

	//衝突時のコールバック
	void OnCollision(const CollisionInfo& info) override;

	void DrawUi();

	DebugText debugText_;
};