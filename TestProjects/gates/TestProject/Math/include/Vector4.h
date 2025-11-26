#pragma once
#include "Vector3.h"
namespace GE
{
	namespace Math
	{
		struct Vector4
		{
			float x, y, z, w;

			/// <summary>
			/// ゼロ初期化コンストラクタ
			/// </summary>
			Vector4();

			/// <summary>
			/// x,y,z,wをxyzwで初期化するコンストラクタ
			/// </summary>
			/// <param name="xyzw">値</param>
			Vector4(float xyzw);

			/// <summary>
			/// x,y,z,wをx,y,z,wで初期化するコンストラクタ
			/// </summary>
			/// <param name="x">値</param>
			/// <param name="y">値</param>
			/// <param name="z">値</param>
			/// <param name="w">値</param>
			Vector4(float x, float y, float z, float w);

			/// <summary>
			/// x,y,zをベクトルで、wをゼロ初期化するコンストラクタ
			/// </summary>
			/// <param name="v">ベクトル</param>
			Vector4(const Vector3& v);

			/// <summary>
			/// このインスタンスの長さを返す関数
			/// </summary>
			/// <returns>長さ</returns>
			float Length();

			/// <summary>
			/// このインスタンスの正規化ベクトルを返す関数
			/// </summary>
			/// <returns>正規化ベクトル</returns>
			Vector4 Normalize();

			/// <summary>
			/// 正規化ベクトルを返す関数
			/// </summary>
			/// <param name="v">ベクトル</param>
			/// <returns>正規化ベクトル</returns>
			static Vector4 Normalize(const Vector4& v);

			Vector4 operator+();
			Vector4 operator-();
			Vector4& operator+=(const Vector4& v);
			Vector4& operator-=(const Vector4& v);
			Vector4& operator*=(float s);
			Vector4& operator/=(float s);
		};
		const Vector4 operator+(const Vector4& v1, const Vector4& v2);
		const Vector4 operator-(const Vector4& v1, const Vector4& v2);
		const Vector4 operator*(const Vector4& v, float s);
		const Vector4 operator*(float s, const Vector4& v);
		const Vector4 operator/(const Vector4& v, float s);
		const Vector4 operator/(float s, const Vector4& v);
	}
}
