#include "GamePlayScene.h"
#include "SafeDelete.h"
#include "Quaternion.h"

#include "Collision.h"
#include <sstream>
#include <iomanip>

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
	//カメラ生成
	camera_ = new Camera();

	planeModel_ = new Model();
	planeModel_ = Model::LoadFromOBJ("plane", false);

	skydomeModel_ = new Model();
	skydomeModel_ = Model::LoadFromOBJ("skydome", true);

	planeObj_ = new Object3d();
	planeObj_ = Object3d::Create();
	planeObj_->SetModel(planeModel_);
	planeObj_->SetScale({ 1, 1, 1 });
	planeObj_->SetCamera(camera_);
	planeObj_->Update();

	skydomeObj_ = new Object3d();
	skydomeObj_ = Object3d::Create();
	skydomeObj_->SetModel(skydomeModel_);
	skydomeObj_->SetScale({ 100, 100, 100 });
	skydomeObj_->SetCamera(camera_);
	skydomeObj_->Update();

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

	//球の初期値
	sphere_.center_ = { 0,2,1 };
	sphere_.radius_ = 1.0f;

	//平面の初期値
	plane_.normal_ = { 0,1,0 };
	plane_.distance_ = 0.0f;

	//三角形の初期値
	triangle_.p0_ = Vector3(-1.0f, 0, -1.0f);//左手前
	triangle_.p1_ = Vector3(-1.0f, 0, +1.0f);//左奥
	triangle_.p2_ = Vector3(+1.0f, 0, -1.0f);//右手前
	triangle_.normal_ = Vector3(0.0f, 1.0f, 0.0f);//上向き
}

void GamePlayScene::Update3d() {
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
	if (input_->PressKey(DIK_W) ||
		input_->PressKey(DIK_S) ||
		input_->PressKey(DIK_D) ||
		input_->PressKey(DIK_A)) {
		if (input_->PressKey(DIK_W)) { camera_->MoveVector({ 0.0f,+1.0f,0.0f }); }
		else if (input_->PressKey(DIK_S)) { camera_->MoveVector({ 0.0f,-1.0f,0.0f }); }
		if (input_->PressKey(DIK_D)) { camera_->MoveVector({ +1.0f,0.0f,0.0f }); }
		else if (input_->PressKey(DIK_A)) { camera_->MoveVector({ -1.0f,0.0f,0.0f }); }
	}

	camera_->Update();
	light_->Update();
	skydomeObj_->Update();
	planeObj_->Update();
}

void GamePlayScene::Update2d() {
	// 現在の座標を取得
	Vector2 position = input_->GetMousePosition();

	//移動後の座標を計算

	// 座標の変更を反映
	sprite_->SetPosition(position);

	sprite_->Update();

	//球移動
	{
		Vector3 moveY = { 0,0.01f,0 };
		Vector3 moveX = { 0.01f,0,0 };

		if (input_->PressKey(DIK_UP)) {
			sphere_.center_ += moveY;
		}
		else if (input_->PressKey(DIK_DOWN)) {
			sphere_.center_ -= moveY;
		}

		if (input_->PressKey(DIK_RIGHT)) {
			sphere_.center_ += moveX;
		}
		else if (input_->PressKey(DIK_LEFT)) {
			sphere_.center_ -= moveX;
		}
	}

	//整形する
	std::ostringstream spherestr;
	spherestr << "Sphere:("
		<< std::fixed << std::setprecision(2)
		<< sphere_.center_.x << ","
		<< sphere_.center_.y << ","
		<< sphere_.center_.z << ")";

	debugText_.Print(spherestr.str(), 50, 180, 1.0f);

	//球と平面の交差判定
	if(false) {
		Vector3 inter;
		//当たり判定
		if (Collision::CheckSphere2Plane(sphere_, plane_, &inter)) {
			debugText_.Print("HIT", 50, 200, 1.0f);

			//交点座標を埋め込む
			spherestr.str("");
			spherestr.clear();
			spherestr << "("
				<< std::fixed << std::setprecision(2)
				<< inter.x << ","
				<< inter.y << ","
				<< inter.z << ")";

			debugText_.Print(spherestr.str(), 50, 220, 1.0f);
		}
	}

	//球と三角形の衝突判定
	if(true) {
		Vector3 inter;
		//当たり判定
		if (Collision::CheckSphere2Triangle(sphere_, triangle_, &inter)) {
			debugText_.Print("HIT", 50, 200, 1.0f);

			//交点座標を埋め込む
			spherestr.str("");
			spherestr.clear();
			spherestr << "("
				<< std::fixed << std::setprecision(2)
				<< inter.x << ","
				<< inter.y << ","
				<< inter.z << ")";

			debugText_.Print(spherestr.str(), 50, 220, 1.0f);
		}
	}
}

void GamePlayScene::Draw3d() {
	skydomeObj_->Draw();
	planeObj_->Draw();
}

void GamePlayScene::Draw2d() {
	sprite_->Draw();
	debugText_.DrawAll();
}

Vector3 GamePlayScene::CreateRotationVector(Vector3 axisAngle, float angleRadian) {
	Quaternion rotation = MakeAxisAngle(axisAngle, ConvertToRadian(1.0f));
	Vector3 point = axisAngle * angleRadian;

	return RotateVector(point, rotation);
}

void GamePlayScene::Finalize() {
	SafeDelete(planeObj_);
	SafeDelete(skydomeObj_);
	SafeDelete(planeModel_);
	SafeDelete(skydomeModel_);
	SafeDelete(sprite_);

	SafeDelete(light_);
	SafeDelete(camera_);
}
