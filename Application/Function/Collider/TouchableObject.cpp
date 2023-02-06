#include "TouchableObject.h"
#include <cassert>

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
