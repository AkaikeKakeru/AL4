#pragma once

#include "Object3d.h"

class TouchableObject :
	public Object3d {
public:
	//オブジェクト生成
	static TouchableObject* Create(Model* model = nullptr);

	//初期化
	bool Initialize(Model* model);
};