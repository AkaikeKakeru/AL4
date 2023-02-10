#pragma once
#include "BaseScene.h"

#include "Input.h"
#include "DrawBasis.h"
#include "Model.h"
#include "Sprite.h"
#include "Object3d.h"

#include "Camera.h"
#include "Light.h"

#include "DebugText.h"

#include "CollisionPrimitive.h"

#include <memory>
#include <List>

class CollisionManager;
class Character;
class Ball;
class TouchableObject;

class GamePlayScene : public BaseScene {
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:
	void Initialize3d();
	void Initialize2d();

	void Update3d();
	void Update2d();

	void Draw3d();
	void Draw2d();
public:
	Vector3 CreateRotationVector(Vector3 axisAngle, float angleRadian);

private:
	//基盤
	static DirectXBasis* dxBas_;
	static Input* input_;
	static DrawBasis* drawBas_;

	DebugText debugText_;

	Camera* camera_ = nullptr;
	Light* light_ = nullptr;

	/// <summary>
	/// オブジェクト
	/// </summary>
	Object3d* skydomeObj_ = nullptr;
	Model* skydomeModel_ = nullptr;

	TouchableObject* groundObj_ = nullptr;
	Model* groundModel_ = nullptr;

	Character* sphereObj_ = nullptr;
	Model* sphereModel_ = nullptr;

	TouchableObject* triangleObj_ = nullptr;
	Model* triangleModel_ = nullptr;

	/// <summary>
	/// スプライト
	/// </summary>
	Sprite* sprite_ = new Sprite();

	/// <summary>
	/// コリダー
	/// </summary>

	float spherePos_[3] = { 0,0,0 };

	//衝突マネージャー
	CollisionManager* collisionManager_ = nullptr;
};