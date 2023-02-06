#include "MeshCollider.h"
#include "Collision.h"
#include <iostream>

void MeshCollider::ConstructTriangle(Model* model) {
	//三角形リストをクリア
	triangles_.clear();

	//モデルの持つメッシュリストを取得
	const std::vector<Mesh*>& meshes = model->GetMeshes();

	//現在のメッシュの開始三角形番号を入れておく変数(0で初期化)
	int start = 0;
	//全メッシュについて順に処理する
	std::vector<Mesh*>::const_iterator it = meshes.cbegin();
	for (; it != meshes.cend(); ++it) {
		Mesh* mesh = *it;

		const std::vector<Mesh::VertexPosNormalUv>& vertices = mesh->GetVertices();
		const std::vector<unsigned short>& indices = mesh->GetIndices();

		//インデックスは、三角形の数×3個あるので、
		//そこからメッシュ内の三角形の数を逆算する
		size_t triangleNum = indices.size() / 3;
	
		//現在のメッシュの三角形の数だけ、三角形リストにスペースを追加する
		triangles_.resize(triangles_.size() + triangleNum);

		//全三角形について順に処理する
		for (int i = 0; i < triangleNum; i++) {
			Triangle& tri = triangles_[start + i];
			int idx0 = indices[i * 3 + 0];
			int idx1 = indices[i * 3 + 1];
			int idx2 = indices[i * 3 + 2];

			//三角形の3頂点の座標を代入
			tri.p0_ = { vertices[idx0].pos.x,vertices[idx0].pos.y,vertices[idx0].pos.z};
			tri.p1_ = { vertices[idx1].pos.x,vertices[idx1].pos.y,vertices[idx1].pos.z};
			tri.p2_ = { vertices[idx2].pos.x,vertices[idx2].pos.y,vertices[idx2].pos.z};
		
			//3頂点から法線を計算
			tri.ComputeNormal();
		}
		//次のメッシュは、今までの三角形番号の次から使う
		start += (int)triangleNum;
	}
}

void MeshCollider::Update() {
	//ワールド行列の逆行列を計算
	invMatWorld_ = Matrix4Inverse(GetObject3d()->GetMatWorld());
}

bool MeshCollider::CheckCollisionSphere(const Sphere& sphere, Vector3* inter) {
	//オブジェクトのローカル座標系での球を得る
	Sphere localSphere;
	localSphere.center_ = Vector3Transform(sphere.center_, invMatWorld_);
	localSphere.radius_ *= Vector3Length({
		invMatWorld_.m[0][0],
		invMatWorld_.m[0][1],
		invMatWorld_.m[0][2]
		});

	//ローカル座標系で交差をチェック
	std::vector<Triangle>::const_iterator it = triangles_.cbegin();

	for (; it != triangles_.cend(); ++it) {
		const Triangle& triangle = *it;

		//球と三角形の衝突判定
		if (Collision::CheckSphere2Triangle(localSphere, triangle, inter)) {
			if (inter) {
				const Matrix4& matWorld = GetObject3d()->GetMatWorld();
				//ワールド座標系での交点を得る
				*inter = Vector3Transform(*inter, matWorld);
			}

			return true;
		}
	}

	return false;
}

