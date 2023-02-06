#include "TouchableObject.h"
#include <cassert>
#include "MeshCollider.h"

TouchableObject* TouchableObject::Create(Model* model) {
	//オブジェクトのインスタンスを生成
	TouchableObject* instance = new TouchableObject();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(model)) {
		delete instance;
		assert(0);
	}

	return instance;
}

bool TouchableObject::Initialize(Model* model) {
	if (!Object3d::Initialize()) {
		return false;
	}

	SetModel(model);

	//コライダーの追加
	MeshCollider* collider = new MeshCollider;
	SetCollider(collider);
	collider->ConstructTriangle(model);

	return true;
}
