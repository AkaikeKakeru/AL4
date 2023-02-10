#pragma once

#include "BaseCollider.h"
#include "CollisionPrimitive.h"

class MeshCollider :
public BaseCollider{
public:
	MeshCollider() {
		//メッシュ形状をセット
		shapeType_ = COLLISIONSHAPE_MESH;
	}

	//三角形の配列を構築する
	void ConstructTriangle(Model* model);

	//更新
	void Update()override;

	//球との衝突判定
	bool CheckCollisionSphere(const Sphere& sphere,
		Vector3* inter = nullptr);

	//レイとの衝突判定
	bool CheckCollisionRay(const Ray& ray, float* distance = nullptr,
		Vector3* inter = nullptr);

private:
	std::vector<Triangle> triangles_;
	//ワールド行列の逆行列
	Matrix4 invMatWorld_{};
};