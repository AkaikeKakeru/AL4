#include "Triangle.h"
#include "Input.h"
#include "MeshCollider.h"


#include <cassert>

#include <sstream>
#include <iomanip>

TriangleObj* TriangleObj::Create(Model* model) {
	//3Dオブジェクトのインスタンス生成
	TriangleObj* instance = new TriangleObj();
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

bool TriangleObj::Initialize() {
	if (!Object3d::Initialize()) {
		return false;
	}

	//コライダーの追加
	float radius = 0.6f;

	//半径分だけ足元から浮いた位置を球の中心にする
	SetCollider(new MeshCollider());

	debugText_.Initialize(0);

	return true;
}

void TriangleObj::Update() {
	Input* input = Input::GetInstance();

	Object3d::Update();
}

void TriangleObj::OnCollision(const CollisionInfo& info) {
	debugText_.Print(
		"Collision detected.",
		10, 300);

	std::ostringstream interstr;

	//交点座標を埋め込む
	interstr << "inter:("
		<< std::fixed << std::setprecision(2)
		<< info.inter_.x << ","
		<< info.inter_.y << ","
		<< info.inter_.z << ")";

	debugText_.Print(interstr.str(), 100, 400, 1.0f);
}

void TriangleObj::DrawUi() {
}
