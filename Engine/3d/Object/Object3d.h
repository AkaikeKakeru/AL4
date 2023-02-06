﻿#pragma once

#include <Windows.h>
#include <wrl.h>

#include "Matrix4.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

#include "Model.h"
#include "WorldTransform.h"
#include "Camera.h"
#include "Light.h"

#include "CollisionInfo.h"

class BaseCollider;

/// <summary>
/// 3Dオブジェクト
/// </summary>
class Object3d {
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // サブクラス
	// パイプラインセット
	struct PipelineSet {
		// ルートシグネチャ
		ComPtr<ID3D12RootSignature> rootsignature_;
		// パイプラインステートオブジェクト
		ComPtr<ID3D12PipelineState> pipelinestate_;
	};

private: // 定数

public: // 静的メンバ関数
	// 静的初期化
	static void StaticInitialize(ID3D12Device* device);

	// 描画前処理
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	// 描画後処理
	static void PostDraw();

	// 3Dオブジェクト生成
	static Object3d* Create();

	static void SetLight(Light* light) {
		Object3d::light_ = light;
	}

private: // 静的メンバ変数
		 // デバイス
	static ID3D12Device* device_;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList_;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature_;
	// テクスチャあり用パイプライン
	static PipelineSet pipelineSet_;

	//ライト
	static Light* light_;

private:// 静的メンバ関数
		// グラフィックパイプライン生成
	static void InitializeGraphicsPipeline();

public: // メンバ関数
	Object3d() = default;
	virtual ~Object3d();

	virtual bool Initialize();
	// 毎フレーム処理
	virtual void Update();
	// 描画
	virtual void Draw();

	//転送
	void TransferMatrixWorld();

	// 座標の取得
	const Vector3& GetPosition() const { return worldTransform_.position_; }

	//スケールの取得
	const Vector3& GetScale() const { return worldTransform_.scale_; }

	//回転の取得
	const Vector3& GetRotation() const { return worldTransform_.rotation_; }

	//ワールド行列の取得
	const Matrix4& GetMatWorld() { return worldTransform_.matWorld_; }


	/// 座標の設定
	void SetPosition(const Vector3& position) { this->worldTransform_.position_ = position; }

	//スケールの設定
	void SetScale(const Vector3& scale) { this->worldTransform_.scale_ = scale; }

	//回転の設定
	void SetRotation(const Vector3& rotation) { this->worldTransform_.rotation_ = rotation; }

	//ワールド変換の設定
	void SetWorldTransform(const WorldTransform worldTransform) { worldTransform_ = worldTransform; }

	//モデルセッタ
	void SetModel(Model* model) { model_ = model; }

	//カメラセット
	void SetCamera(Camera* camera) { camera_ = camera; }

	//コライダーのセット
	void SetCollider(BaseCollider* collider);


	//衝突時のコールバック
	virtual void OnCollision(const CollisionInfo& info){}

protected: // メンバ変数
	//ワールドトランスフォーム
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	//カメラ
	Camera* camera_ = nullptr;

	//クラス名(デバッグ用)
	const char* name_ = nullptr;

	//コライダー
	BaseCollider* collider_ = nullptr;
};