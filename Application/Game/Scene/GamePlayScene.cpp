#include "GamePlayScene.h"
#include "SafeDelete.h"
#include "Quaternion.h"

#include "Character.h"

#include "Collision.h"
#include "CollisionManager.h"
#include "SphereCollider.h"

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
	collisionManager_ = CollisionManager::GetInstance();

	//カメラ生成
	camera_ = new Camera();

	planeModel_ = new Model();
	planeModel_ = Model::LoadFromOBJ("plane", false);

	skydomeModel_ = new Model();
	skydomeModel_ = Model::LoadFromOBJ("skydome", true);

	sphereModel_ = new Model();
	sphereModel_ = Model::LoadFromOBJ("sphere", true);

	planeObj_ = Character::Create(planeModel_);
	planeObj_->SetScale({ 1, 1, 1 });
	planeObj_->SetCamera(camera_);
	planeObj_->SetCollider(new SphereCollider);
	planeObj_->Update();

	sphereObj_ = Object3d::Create();
	sphereObj_->SetModel(sphereModel_);
	sphereObj_->SetScale({ 1, 1, 1 });
	sphereObj_->SetPosition({ -5,0,0 });
	sphereObj_->SetCamera(camera_);
	sphereObj_->SetCollider(new SphereCollider);
	sphereObj_->Update();

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
	triangle_.p0_ = {-1.0f, 0, -1.0f};//左手前
	triangle_.p1_ = {-1.0f, 0, +1.0f};//左奥
	triangle_.p2_ = { +1.0f, 0, -1.0f };//右手前
	triangle_.normal_ = { 0.0f, 1.0f, 0.0f };//上向き

	//レイの初期値
	ray_.start_ = { 0,2,0 };//原点のやや上
	ray_.dir_ = { 0,-1,0 };//下向き

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

	sphereObj_->Update();
	planeObj_->Update();

	//全ての衝突判定をチェック
	collisionManager_->CheckAllCollision();
}

void GamePlayScene::Update2d() {
	// 現在の座標を取得
	Vector2 position = input_->GetMousePosition();

	//移動後の座標を計算

	// 座標の変更を反映
	sprite_->SetPosition(position);

	sprite_->Update();

	//球移動
	if(false) {
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

	debugText_.Print(spherestr.str(), 50, 20, 1.0f);

	//レイ移動
	if(true) {
		Vector3 moveZ = { 0,0,0.01f };
		Vector3 moveX = { 0.01f,0,0 };

		if (input_->PressKey(DIK_UP)) {
			ray_.start_ += moveZ;
		}
		else if (input_->PressKey(DIK_DOWN)) {
			ray_.start_ -= moveZ;
		}

		if (input_->PressKey(DIK_RIGHT)) {
			ray_.start_ += moveX;
		}
		else if (input_->PressKey(DIK_LEFT)) {
			ray_.start_ -= moveX;
		}
	}

	//整形する
	std::ostringstream raystr;
	raystr << "ray_start:("
		<< std::fixed << std::setprecision(2)
		<< ray_.start_.x << ","
		<< ray_.start_.y << ","
		<< ray_.start_.z << ")";

	debugText_.Print(raystr.str(), 50, 180, 1.0f);

	//球と平面の交差判定
	if(false) {
		Vector3 inter;
		//当たり判定
		if (Collision::CheckSphere2Plane(sphere_, plane_, &inter)) {
			debugText_.Print("HIT", 50, 60, 1.0f);

			//交点座標を埋め込む
			spherestr.str("");
			spherestr.clear();
			spherestr << "("
				<< std::fixed << std::setprecision(2)
				<< inter.x << ","
				<< inter.y << ","
				<< inter.z << ")";

			debugText_.Print(spherestr.str(), 50, 80, 1.0f);
		}
	}

	//球と三角形の衝突判定
	if(false) {
		Vector3 inter;
		//当たり判定
		if (Collision::CheckSphere2Triangle(sphere_, triangle_, &inter)) {
			debugText_.Print("HIT", 50, 120, 1.0f);

			//交点座標を埋め込む
			spherestr.str("");
			spherestr.clear();
			spherestr << "("
				<< std::fixed << std::setprecision(2)
				<< inter.x << ","
				<< inter.y << ","
				<< inter.z << ")";

			debugText_.Print(spherestr.str(), 50, 140, 1.0f);
		}
	}

	//レイと平面の衝突判定
	if(false) {
		Vector3 inter;
		float distance;
		//当たり判定
		if (Collision::CheckRay2Plane(ray_, plane_,&distance, &inter)) {
			debugText_.Print("HIT", 50, 200, 1.0f);

			//交点座標を埋め込む
			raystr.str("");
			raystr.clear();
			raystr << "("
				<< std::fixed << std::setprecision(2)
				<< inter.x << ","
				<< inter.y << ","
				<< inter.z << ")";

			debugText_.Print(raystr.str(), 50, 220, 1.0f);
		}
	}

	//レイと三角形の衝突判定
	if(false) {
		Vector3 inter;
		float distance;
		//当たり判定
		if (Collision::CheckRay2Triangle(ray_, triangle_,&distance, &inter)) {
			debugText_.Print("HIT", 50, 260, 1.0f);

			//交点座標を埋め込む
			raystr.str("");
			raystr.clear();
			raystr << "inter:("
				<< std::fixed << std::setprecision(2)
				<< inter.x << ","
				<< inter.y << ","
				<< inter.z << ")";

			debugText_.Print(raystr.str(), 50, 280, 1.0f);

			raystr.str("");
			raystr.clear();
			raystr << "distance:("
				<< std::fixed << std::setprecision(2)
				<< distance << ")";

			debugText_.Print(raystr.str(), 50, 300, 1.0f);
		}
	}

	//レイと球の衝突判定
	if(true) {
		Vector3 inter;
		float distance;
		//当たり判定
		if (Collision::CheckRay2Sphere(ray_, sphere_,&distance, &inter)) {
			debugText_.Print("HIT", 50, 340, 1.0f);

			//交点座標を埋め込む
			raystr.str("");
			raystr.clear();
			raystr << "inter:("
				<< std::fixed << std::setprecision(2)
				<< inter.x << ","
				<< inter.y << ","
				<< inter.z << ")";

			debugText_.Print(raystr.str(), 50, 360, 1.0f);

			raystr.str("");
			raystr.clear();
			raystr << "distance:("
				<< std::fixed << std::setprecision(2)
				<< distance << ")";

			debugText_.Print(raystr.str(), 50, 380, 1.0f);
		}
	}
}

void GamePlayScene::Draw3d() {
	skydomeObj_->Draw();
	sphereObj_->Draw();
	planeObj_->Draw();
}

void GamePlayScene::Draw2d() {
	sprite_->Draw();
	debugText_.DrawAll();
	planeObj_->DrawUi();
}

Vector3 GamePlayScene::CreateRotationVector(Vector3 axisAngle, float angleRadian) {
	Quaternion rotation = MakeAxisAngle(axisAngle, ConvertToRadian(1.0f));
	Vector3 point = axisAngle * angleRadian;

	return RotateVector(point, rotation);
}

void GamePlayScene::Finalize() {
	SafeDelete(planeObj_);
	SafeDelete(skydomeObj_);
	SafeDelete(sphereObj_);

	SafeDelete(planeModel_);
	SafeDelete(skydomeModel_);
	SafeDelete(sphereModel_);

	SafeDelete(sprite_);

	SafeDelete(light_);
	SafeDelete(camera_);
}
