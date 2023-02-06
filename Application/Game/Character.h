#pragma once

#include "Object3d.h"
#include "DebugText.h"

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

	void DrawUi();

	DebugText debugText_;

private:
	//接地フラグ
	bool onGround_ = true;
	//落下ベクトル
	Vector3 fallV_;
};