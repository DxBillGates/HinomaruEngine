#pragma once
#include "Math.h"

namespace GE
{
	struct Transform
	{
		Math::Vector3 position = {};
		Math::Quaternion rotation = {};
		Math::Vector3 scale = { 1,1,1 };


		/// <summary>
		/// コンストラクタ
		/// </summary>
		Transform()
		{
		}


		/// <summary>
		/// 位置、回転、サイズを指定するコンストラクタ
		/// </summary>
		/// <param name="pos">位置</param>
		/// <param name="rotation">回転</param>
		/// <param name="scale">サイズ</param>
		Transform(const Math::Vector3& pos, const Math::Vector3& rotation, const Math::Vector3& scale)
			: position(pos)
			, rotation(rotation,0)
			, scale(scale)
		{
		}

		/// <summary>
		/// Transformを指定するコンストラクタ
		/// </summary>
		/// <param name="transform">コピーするTransform</param>
		Transform(const Transform& transform)
			: position(transform.position)
			, rotation(transform.rotation)
			, scale(transform.scale)
		{
		}

		/// <summary>
		/// 行列を返す関数
		/// </summary>
		/// <returns>行列</returns>
		Math::Matrix4x4 GetMatrix();

		/// <summary>
		/// オイラー角を返す関数
		/// </summary>
		/// <returns>オイラー角</returns>
		Math::Vector3 GetEulerAngle();

		/// <summary>
		/// 前方ベクトルを返す関数
		/// </summary>
		/// <returns>前方ベクトル</returns>
		Math::Vector3 GetForward();

		/// <summary>
		/// 右側ベクトルを返す関数
		/// </summary>
		/// <returns>右側ベクトル</returns>
		Math::Vector3 GetRight();

		/// <summary>
		/// 上ベクトルを返す関数
		/// </summary>
		/// <returns>上ベクトル</returns>
		Math::Vector3 GetUp();
	};
}