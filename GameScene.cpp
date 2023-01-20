#include "GameScene.h"
#include <cassert>

using namespace DirectX;

template <class T>
inline void safe_delete(T*& p) {
	delete p;
	p = nullptr;
}

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	//オブジェクトの解放
	safe_delete(objectTriangle_);
	safe_delete(objectPlane_);
	safe_delete(objectRobot_);

	safe_delete(objectSkydome_);
	safe_delete(objectGround_);

	//モデルの解放
	safe_delete(modelTriangle_);
	safe_delete(modelPlane_);
	safe_delete(modelRobot_);

	safe_delete(modelSkydome_);
	safe_delete(modelGround_);

	//スプライトの解放
	//safe_delete(spriteBG_);
	safe_delete(sprite1_);
	safe_delete(sprite2_);
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input)
{
	// nullptrチェック
	assert(dxCommon);
	assert(input);

	this->dxCommon_ = dxCommon;
	this->input_ = input;

	// デバッグテキスト用テクスチャ読み込み
	Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png");
	// デバッグテキスト初期化
	debugText_.Initialize(debugTextTexNumber);

	// テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/background.png");

	//テクスチャ2番に読み込み
	Sprite::LoadTexture(2, L"Resources/texture.png");

	//座標{0,0}に、テクスチャ2番のスプライトを生成
	sprite1_ = Sprite::Create(2, { 0,0 });
	//座標{500,500}に、テクスチャ2番のスプライトを生成
	sprite2_ = Sprite::Create(2, { 500,500 }, { 1,0,0,1 }, { 0,0 }, false, true);

	// 背景スプライト生成
	//spriteBG_ = Sprite::Create(1, { 0.0f,0.0f });

	//3Dモデル読み込み
	modelTriangle_ = Model::LoadFromOBJ("triangle_mat");
	modelPlane_ = Model::LoadFromOBJ("plane");
	modelRobot_ = Model::LoadFromOBJ("robot");
	modelSkydome_ = Model::LoadFromOBJ("skydome");
	modelGround_ = Model::LoadFromOBJ("ground");

	// 3Dオブジェクト生成
	objectTriangle_ = Object3d::Create();
	objectTriangle_->SetModel(modelTriangle_);
	objectTriangle_->Update();

	objectPlane_ = Object3d::Create();
	objectPlane_->SetModel(modelPlane_);
	objectPlane_->SetPosition({ 0,20,0 });
	objectPlane_->Update();

	objectRobot_ = Object3d::Create();
	objectRobot_->SetModel(modelRobot_);
	objectRobot_->Update();

	objectSkydome_ = Object3d::Create();
	objectSkydome_->SetModel(modelSkydome_);
	objectSkydome_->SetScale({200, 200, 200});
	objectSkydome_->Update();

	objectGround_ = Object3d::Create();
	objectGround_->SetModel(modelGround_);
	objectGround_->SetScale({200, 200, 200});
	objectGround_->Update();
}

void GameScene::Update()
{
	// オブジェクト移動
	if (input_->PushKey(DIK_UP) || input_->PushKey(DIK_DOWN) || input_->PushKey(DIK_RIGHT) || input_->PushKey(DIK_LEFT))
	{
		// 現在の座標を取得
		Vector3 position = objectPlane_->GetPosition();

		// 移動後の座標を計算
		if (input_->PushKey(DIK_UP)) { position.y += 1.0f; }
		else if (input_->PushKey(DIK_DOWN)) { position.y -= 1.0f; }
		if (input_->PushKey(DIK_RIGHT)) { position.x += 1.0f; }
		else if (input_->PushKey(DIK_LEFT)) { position.x -= 1.0f; }

		// 座標の変更を反映
		objectPlane_->SetPosition(position);
	}

	// カメラ移動
	if (input_->PushKey(DIK_W) || 
		input_->PushKey(DIK_S) || 
		input_->PushKey(DIK_D) || 
		input_->PushKey(DIK_A) ||
		input_->PushKey(DIK_Q) || 
		input_->PushKey(DIK_C) )
	{
		if (input_->PushKey(DIK_W)) { Object3d::CameraMoveVector({ 0.0f,+1.0f,0.0f }); }
		else if (input_->PushKey(DIK_S)) { Object3d::CameraMoveVector({ 0.0f,-1.0f,0.0f }); }
		if (input_->PushKey(DIK_D)) { Object3d::CameraMoveVector({ +1.0f,0.0f,0.0f }); }
		else if (input_->PushKey(DIK_A)) { Object3d::CameraMoveVector({ -1.0f,0.0f,0.0f }); }
		if (input_->PushKey(DIK_Q)) { Object3d::CameraMoveVector({ 0.0f,0.0f,+1.0f }); }
		else if (input_->PushKey(DIK_C)) { Object3d::CameraMoveVector({ 0.0f,0.0f,-1.0f }); }

	}

	// オブジェクト移動
	if (input_->PushKey(DIK_SPACE))
	{
		// 現在の座標を取得
		XMFLOAT2 position = sprite1_->GetPosition();

		// 移動後の座標を計算
		position.x += 1.0f;

		// 座標の変更を反映
		sprite1_->SetPosition(position);
	}

	objectGround_->Update();
	objectSkydome_->Update();

	objectTriangle_->Update();
	objectPlane_->Update();
	objectRobot_->Update();
}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
//	spriteBG_->Draw();

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(cmdList);

	// 3Dオブクジェクトの描画
	objectGround_->Draw();
	objectSkydome_->Draw();

	objectTriangle_->Draw();
	objectPlane_->Draw();
	objectRobot_->Draw();

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	//sprite1->Draw();
	//sprite2->Draw();

	// デバッグテキストの描画
	debugText_.DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion

}

