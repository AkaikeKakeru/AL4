#pragma once

#include "CollisionPrimitive.h"

class Collision {
public:

	/// <summary>
	///球と球の衝突判定
	/// </summary>
	/// <param name="posA">A位置</param>
	/// <param name="posB">B位置</param>
	/// <param name="radA">A半径</param>
	/// <param name="radB">B半径</param>
	/// <returns>衝突しているか</returns>
	static bool CheckSphere2Sphere(
		Vector3 posA,Vector3 posB,float radA,float radB);

	/// <summary>
	//球と平面の交差判定
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="plane">平面</param>
	/// <param name="inter">交点</param>
	/// <returns>交差しているか</returns>
	static bool CheckSphere2Plane(
		const Sphere& sphere, const Plane& plane, Vector3* inter = nullptr);

	/// <summary>
	//点と三角形の最近接点を求める
	/// </summary>
	/// <param name="point">点</param>
	/// <param name="triangle">三角形</param>
	/// <param name="closest">最近接点(出力用)</param>
	static void ClosessPtPoint2Triangle(
		const Vector3& point, const Triangle& triangle, Vector3* closest);

	/// <summary>
	//球と法線付き三角形の衝突判定
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="triangle">三角形</param>
	/// <param name="inter">交点(三角形上の最近接点)</param>
	/// <returns>衝突しているか</returns>
	static bool CheckSphere2Triangle(
		const Sphere& sphere, const Triangle& triangle, Vector3* inter = nullptr);

	/// <summary>
	/// レイと平面の衝突判定
	/// </summary>
	/// <param name="ray">レイ</param>
	/// <param name="plane">平面</param>
	/// <param name="distance">距離(出力)</param>
	/// <param name="inter">交点(出力用)</param>
	/// <returns>衝突しているか</returns>
	static bool CheckRay2Plane(const Ray& ray, const Plane& plane,
		float* distance = nullptr, Vector3* inter = nullptr);
};