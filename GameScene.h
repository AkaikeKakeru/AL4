#pragma once

#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Input.h"
#include "Sprite.h"
#include "Object3d.h"
#include "Model.h"
#include "DebugText.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private: // 静的メンバ変数
	static const int debugTextTexNumber = 0;

public: // メンバ関数

	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DirectXCommon* dxCommon, Input* input);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	DebugText debugText_;	

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	Sprite* spriteBG_ = nullptr;
	Object3d* objectTriangle_ = nullptr;
	Object3d* objectPlane_ = nullptr;
	Object3d* objectRobot_ = nullptr;

	Model* modelTriangle_ = nullptr;
	Model* modelPlane_ = nullptr;
	Model* modelRobot_ = nullptr;

private: // メンバ変数
	Sprite* sprite1_ = nullptr;
	Sprite* sprite2_ = nullptr;
};

