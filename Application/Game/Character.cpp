#include "Character.h"
#include "Input.h"
#include "SphereCollider.h"
#include <cassert>



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

	Object3d::Update();
}

void Character::OnCollision(const CollisionInfo& info) {
	debugText_.Print(
		"Collision detected.",
		10,300);
}

void Character::DrawUi() {
	debugText_.DrawAll();
}
