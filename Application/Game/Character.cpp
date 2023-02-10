#include "Character.h"
#include "Input.h"
#include "SphereCollider.h"
#include <cassert>

#include "Collision.h"

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

	debugText_.Initialize(0);

	return true;
}

void Character::Update() {
	Input* input = Input::GetInstance();
	//旋回スピード
	float rotaSpeed = ConvertToRadian(2.0f);
	//移動ベクトル
	Vector3 move = { 0,0,0.1f };
	//上昇下降スピード
	float speed = 0.2f;

	//上昇下降
	if (input->PressKey(DIK_UP)) {
		worldTransform_.position_.y += speed;
	}
	else if (input->PressKey(DIK_DOWN)) {
		worldTransform_.position_.y -= speed;
	}


	//A,Dで左右
	if (input->PressKey(DIK_LEFT)) {
		worldTransform_.position_.x -= speed;
	}
	else if (input->PressKey(DIK_RIGHT)) {
		worldTransform_.position_.x += speed;
	}

	Object3d::Update();
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
