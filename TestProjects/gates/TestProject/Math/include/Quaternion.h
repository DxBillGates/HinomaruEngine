#pragma once
#include "Vector3.h"
#include "Matrix4.h"
namespace GE
{
	namespace Math
	{
		struct Quaternion
		{
			union
			{
				struct
				{
					float x;
					float y;
					float z;
					float w;
				};

				float value[4];
			};


			/// <summary>
			/// コンストラクタ
			/// </summary>
			Quaternion();

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="x">値</param>
			/// <param name="y">値</param>
			/// <param name="z">値</param>
			/// <param name="w">値</param>
			Quaternion(float x, float y, float z, float w = 1.0f);

			/// <summary>
			/// 軸とアングルからクォータニオンを生成するコンストラクタ
			/// </summary>
			/// <param name="v">軸</param>
			/// <param name="angle">ラジアン</param>
			Quaternion(const Vector3& v, float angle);

			/// <summary>
			/// 行列からクォータニオンを生成するコンストラクタ
			/// </summary>
			/// <param name="m">行列</param>
			Quaternion(const Matrix4x4& m);

			/// <summary>
			/// 正規化
			/// </summary>
			/// <param name="q">クォータニオン</param>
			/// <returns>正規化クォータニオン</returns>
			static Quaternion Normalize(const Quaternion& q);

			/// <summary>
			/// 共役を生成する関数
			/// </summary>
			/// <param name="q">クォータニオン</param>
			/// <returns>共役</returns>
			static Quaternion Conjugate(const Quaternion& q);

			/// <summary>
			/// クォータニオン同士の内積を返す関数
			/// </summary>
			/// <param name="q1">クォータニオン</param>
			/// <param name="q2">クォータニオン</param>
			/// <returns>内積結果</returns>
			static float Dot(const Quaternion& q1, const Quaternion& q2);

			/// <summary>
			/// クォータニオンの長さを返す関数
			/// </summary>
			/// <param name="q">クォータニオン</param>
			/// <returns>長さ</returns>
			static float Length(const Quaternion& q);

			/// <summary>
			/// 2つのクォータニオンから球面線形補間を生成する関数
			/// </summary>
			/// <param name="q1">クォータニオン</param>
			/// <param name="q2">クォータニオン</param>
			/// <param name="t">タイム 0 ~ 1</param>
			/// <returns>球面線形補間</returns>
			static Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t);

			/// <summary>
			/// 2つのクォータニオンから線形補間を生成する関数
			/// </summary>
			/// <param name="q1">クォータニオン</param>
			/// <param name="q2">クォータニオン</param>
			/// <param name="t">タイム 0 ~ 1</param>
			/// <returns>線形補間</returns>
			static Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, float t);

			/// <summary>
			/// クォータニオンから回転行列を生成する関数
			/// </summary>
			/// <param name="q">クォータニオン</param>
			/// <returns>回転行列</returns>
			static Matrix4x4 Rotation(const Quaternion& q);

			/// <summary>
			/// セットされてるクォータニオンから回転行列を作成する
			/// </summary>
			/// <returns>回転行列</returns>
			Matrix4x4 Rotation();

			/// <summary>
			/// クォータニオンの軸を返す関数
			/// </summary>
			/// <param name="q">クォータニオン</param>
			/// <returns>軸ベクトル</returns>
			static Vector3 GetAxis(const Quaternion& q);

			static Quaternion Euler(const Math::Vector3& angle);

			Quaternion operator+();
			Quaternion operator-();
			Quaternion& operator+=(const Quaternion& q);
			Quaternion& operator-=(const Quaternion& q);
			Quaternion& operator*=(float s);
			Quaternion& operator/=(float s);
			Quaternion& operator*=(const Quaternion& q);
		};
		Quaternion operator+(const Quaternion& q1, const Quaternion& q2);
		Quaternion operator-(const Quaternion& q1, const Quaternion& q2);
		Quaternion operator*(const Quaternion& q1, const Quaternion& q2);
		Quaternion operator*(const Quaternion& q, float s);
		Quaternion operator*(float s, const Quaternion& q);
		Quaternion operator/(const Quaternion& q, float s);
	}
}