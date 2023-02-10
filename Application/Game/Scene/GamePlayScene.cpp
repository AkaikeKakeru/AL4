#include "GamePlayScene.h"
#include "SafeDelete.h"
#include "Quaternion.h"

#include "Character.h"
#include "Ball.h"

#include "Collision.h"
#include "CollisionManager.h"
#include "SphereCollider.h"

#include "MeshCollider.h"
#include "TouchableObject.h"

#include <sstream>
#include <iomanip>


#include <imgui.h>


DirectXBasis* GamePlayScene::dxBas_ = DirectXBasis::GetInstance();
Input* GamePlayScene::input_ = Input::GetInstance();
DrawBasis* GamePlayScene::drawBas_ = DrawBasis::GetInstance();

void GamePlayScene::Initialize() {
	Initialize2d();
	Initialize3d();
}

void GamePlayScene::Update() {
	input_->Update();

	Update3d();
	Update2d();
}

void GamePlayScene::Draw() {
	Object3d::PreDraw(dxBas_->GetCommandList().Get());
	Draw3d();
	Object3d::PostDraw();

	drawBas_->PreDraw();
	Draw2d();
	drawBas_->PostDraw();
}

void GamePlayScene::Initialize3d() {
	collisionManager_ = CollisionManager::GetInstance();

	//カメラ生成
	camera_ = new Camera();

	robotModel_ = new Model();
	robotModel_ = Model::LoadFromOBJ("robot", false);

	skydomeModel_ = new Model();
	skydomeModel_ = Model::LoadFromOBJ("skydome", true);

	sphereModel_ = new Model();
	sphereModel_ = Model::LoadFromOBJ("sphere", true);

	groundModel_ = new Model();
	groundModel_ = Model::LoadFromOBJ("ground", true);

	robotObj_ = Character::Create(robotModel_);
	robotObj_->SetScale({ 1, 1, 1 });
	robotObj_->SetPosition({ 0,0,0 });
	robotObj_->SetCamera(camera_);
	robotObj_->SetCollider(new SphereCollider);
	robotObj_->Update();

	sphereObj_ = Ball::Create(sphereModel_);
	sphereObj_->SetScale({ 1, 1, 1 });
	sphereObj_->SetPosition({ 0,0,0 });
	sphereObj_->SetCamera(camera_);
	sphereObj_->SetCollider(new SphereCollider);
	sphereObj_->Update();

	skydomeObj_ = Object3d::Create();
	skydomeObj_->SetModel(skydomeModel_);
	skydomeObj_->SetScale({ 100, 100, 100 });
	skydomeObj_->SetCamera(camera_);
	skydomeObj_->Update();

	groundObj_ = TouchableObject::Create(groundModel_);
	groundObj_->SetCamera(camera_);
	groundObj_->Update();

	//ライト生成
	light_ = new Light();
	light_ = Light::Create();
	light_->SetLightDir({ 0,-1,1 });
	light_->SetLightColor({ 1,1,1 });
	light_->Update();
	Object3d::SetLight(light_);
}

void GamePlayScene::Initialize2d() {

	drawBas_->LoadTexture(0, "debugfont.png");
	debugText_.Initialize(0);

	drawBas_->LoadTexture(1, "texture.png");
	sprite_->Initialize(drawBas_, 1);

	sprite_->SetAnchorPoint({ 0.5f, 0.5f });
	sprite_->SetSize({ 64,64 });


}

void GamePlayScene::Update3d() {
	{
		spherePos_[0] = sphereObj_->GetPosition().x;
		spherePos_[1] = sphereObj_->GetPosition().y;
		spherePos_[2] = sphereObj_->GetPosition().z;
	}

	// オブジェクト移動
	//if (input_->PressKey(DIK_UP) ||
	//	input_->PressKey(DIK_DOWN) ||
	//	input_->PressKey(DIK_RIGHT) ||
	//	input_->PressKey(DIK_LEFT)) {
	//	// 現在の座標を取得
	//	Vector3 position = planeObj_->GetPosition();

	//	//移動スピード
	//	float moveSpeed = 0.5f;

	//	//移動後の座標を計算
	//	if (input_->PressKey(DIK_UP)) {
	//		// 移動後の座標を計算
	//		position.y += moveSpeed;
	//	}

	//	else if (input_->PressKey(DIK_DOWN)) {
	//		position.y -= moveSpeed;
	//	}

	//	if (input_->PressKey(DIK_RIGHT)) {
	//		position.x += moveSpeed;
	//	}

	//	else if (input_->PressKey(DIK_LEFT)) {
	//		position.x -= moveSpeed;
	//	}

	//	// 座標の変更を反映
	//	planeObj_->SetPosition(position);
	//}

	// カメラ移動
	//if (input_->PressKey(DIK_W) ||
	//	input_->PressKey(DIK_S) ||
	//	input_->PressKey(DIK_D) ||
	//	input_->PressKey(DIK_A)) {
	//	if (input_->PressKey(DIK_W)) { camera_->MoveVector({ 0.0f,+1.0f,0.0f }); }
	//	else if (input_->PressKey(DIK_S)) { camera_->MoveVector({ 0.0f,-1.0f,0.0f }); }
	//	if (input_->PressKey(DIK_D)) { camera_->MoveVector({ +1.0f,0.0f,0.0f }); }
	//	else if (input_->PressKey(DIK_A)) { camera_->MoveVector({ -1.0f,0.0f,0.0f }); }
	//}

	camera_->Update();
	light_->Update();
	skydomeObj_->Update();
	groundObj_->Update();

	sphereObj_->Update();
	robotObj_->Update();

	//全ての衝突判定をチェック
	collisionManager_->CheckAllCollision();
}

void GamePlayScene::Update2d() {
}

void GamePlayScene::Draw3d() {
	skydomeObj_->Draw();
	groundObj_->Draw();
	sphereObj_->Draw();
	robotObj_->Draw();
}

void GamePlayScene::Draw2d() {
	//sprite_->Draw();
	debugText_.DrawAll();
	robotObj_->DrawUi();
	sphereObj_->DrawUi();
}

Vector3 GamePlayScene::CreateRotationVector(Vector3 axisAngle, float angleRadian) {
	Quaternion rotation = MakeAxisAngle(axisAngle, ConvertToRadian(1.0f));
	Vector3 point = axisAngle * angleRadian;

	return RotateVector(point, rotation);
}

void GamePlayScene::Finalize() {
	SafeDelete(robotObj_);
	SafeDelete(skydomeObj_);
	SafeDelete(sphereObj_);
	SafeDelete(groundObj_);

	SafeDelete(robotModel_);
	SafeDelete(skydomeModel_);
	SafeDelete(sphereModel_);
	SafeDelete(groundModel_);

	SafeDelete(sprite_);

	SafeDelete(light_);
	SafeDelete(camera_);
}
