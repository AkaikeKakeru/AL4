#pragma once

#include "Object3d.h"
#include "DebugText.h"

class TriangleObj :
	public Object3d {
public:
	//オブジェクト生成
	static TriangleObj* Create(Model* model = nullptr);
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