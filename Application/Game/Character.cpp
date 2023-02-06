#include "Character.h"
#include "Input.h"

#include "SphereCollider.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"

#include <cassert>

#include <sstream>
#include <iomanip>

Character* Character::Create(Model* model) {
	//3Dオブジェクトのインスタンス生成
	Character* instance = new Character();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
	}

	//モデルをセット
	if (model) {
		instance->SetModel(model);
	}

	return instance;
}

bool Character::Initialize() {
	if (!Object3d::Initialize()) {
		return false;
	}

	//コライダーの追加
	float radius = 0.6f;

	//半径分だけ足元から浮いた位置を球の中心にする
	SetCollider(new SphereCollider({ 0,radius,0 },radius));

	collider_->SetAttribute(COLLISION_ATTR_ALLIES);

	debugText_.Initialize(0);
	return true;
}

void Character::Update() {
	Input* input = Input::GetInstance();
	//旋回スピード
	float rotaSpeed = ConvertToRadian(2.0f);
	//移動ベクトル
	Vector3 move = { 0,0,0.1f };

	//A,Dで旋回
	if (input->PressKey(DIK_A)) {
		worldTransform_.rotation_.y -= rotaSpeed;
	}
	else if (input->PressKey(DIK_D)) {
		worldTransform_.rotation_.y += rotaSpeed;
	}

	Matrix4 matRot =
		Matrix4RotationY(worldTransform_.rotation_.y);
	move = Vector3Normalize(Vector3Transform(move, matRot)) / 8;

	//向いている方向に移動
	if (input->PressKey(DIK_S)) {
		worldTransform_.position_ -= move;
	}
	else if (input->PressKey(DIK_W)) {
		worldTransform_.position_ += move;
	}

	//落下処理
	if (!onGround_) {
		//下向き加速度
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.05f;

		//加速
		fallV_.y = max(fallV_.y + fallAcc, fallVYMin);

		//移動
		worldTransform_.position_.x += fallV_.x;
		worldTransform_.position_.y += fallV_.y;
		worldTransform_.position_.z += fallV_.z;
	}
	//ジャンプ操作
	else if (input->TriggerKey(DIK_SPACE)) {
		onGround_ = false;
		const float jumpVYFist = 0.2f;//ジャンプ時上向き初速
		fallV_ = { 0,jumpVYFist,0 };
	}

	//オブジェクトの更新
	Object3d::Update();

	///着地処理

	//球コライダーを取得
	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(collider_);
	assert(sphereCollider);

	//球の上端から球の下端までのレイキャスト用レイを準備
	Ray ray;
	ray.start_ = sphereCollider->center_;
	ray.start_.y += sphereCollider->GetRadius();
	ray.dir_ = { 0,-1,0 };
	RaycastHit raycastHit;

	//接地状態
	if (onGround_) {
		//スムーズに坂を下るための吸着距離
		const float adsDistance = 0.2f;
		//接地を維持
		if (CollisionManager::GetInstance()->Raycast(
			ray, COLLISION_ATTR_LANDSHAPE,
			&raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance)) {
			onGround_ = true;
			worldTransform_.position_.y -= (raycastHit.distance_ - sphereCollider->GetRadius() * 2.0f);
			//オブジェクトの更新
			Object3d::Update();
		}
		//地面がないので落下
		else {
			onGround_ = false;
			fallV_ = {};
		}
	}
}

void Character::OnCollision(const CollisionInfo& info) {
	debugText_.Print(
		"Collision detected.",
		10,300);

	std::ostringstream interstr;

		//交点座標を埋め込む
		interstr << "inter:("
			<< std::fixed << std::setprecision(2)
			<< info.inter_.x << ","
			<< info.inter_.y << ","
			<< info.inter_.z << ")";

		debugText_.Print(interstr.str(), 50, 400, 1.0f);
}

void Character::DrawUi() {
	debugText_.DrawAll();
}
