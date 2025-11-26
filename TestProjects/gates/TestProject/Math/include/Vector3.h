#pragma once
namespace GE
{
	namespace Math
	{
		struct Vector3
		{
			union
			{
				struct
				{
					float x;
					float y;
					float z;
				};
				float value[3];
			};

			/// <summary>
			/// ゼロ初期化コンストラクタ
			/// </summary>
			Vector3();

			/// <summary>
			/// x,y,zをxyzで初期化するコンストラクタ
			/// </summary>
			/// <param name="xyz"></param>
			Vector3(float xyz);

			/// <summary>
			/// x,y,zをx,y,zで初期化するコンストラクタ
			/// </summary>
			/// <param name="x"></param>
			/// <param name="y"></param>
			/// <param name="z"></param>
			Vector3(float x, float y, float z);

			/// <summary>
			/// このインスタンスの長さを返す関数
			/// </summary>
			/// <returns>長さ</returns>
			float Length() const;

			/// <summary>
			/// このインスタンスの正規化ベクトルを返す関数
			/// </summary>
			/// <returns></returns>
			Vector3 Normalize();

			/// <summary>
			/// 2つのベクトルからなるベクトルの長さを返す関数
			/// </summary>
			/// <param name="v1">ベクトル</param>
			/// <param name="v2">ベクトル</param>
			/// <returns>長さ</returns>
			static float Distance(const Vector3& v1, const Vector3& v2);

			/// <summary>
			/// 正規化ベクトルを返す関数
			/// </summary>
			/// <param name="v">ベクトル</param>
			/// <returns>正規化ベクトル</returns>
			static Vector3 Normalize(const Vector3& v);

			/// <summary>
			/// 2つのベクトルの外積を返す関数
			/// </summary>
			/// <param name="v1">ベクトル</param>
			/// <param name="v2">ベクトル</param>
			/// <returns>外積</returns>
			static Vector3 Cross(const Vector3& v1, const Vector3& v2);

			/// <summary>
			/// 2つのベクトルの内積を返す関数
			/// </summary>
			/// <param name="v1">ベクトル</param>
			/// <param name="v2">ベクトル</param>
			/// <returns>内積</returns>
			static float Dot(const Vector3& v1, const Vector3& v2);

			/// <summary>
			/// 2つのベクトルの線形補間を返す関数
			/// </summary>
			/// <param name="s">スタート位置</param>
			/// <param name="e">終了位置</param>
			/// <param name="t">タイム 0 ~ 1</param>
			/// <returns>線形補間結果</returns>
			static Vector3 Lerp(const Vector3& s, const Vector3& e, float t);

			/// <summary>
			/// std::max()の3次元ベクトル版
			/// </summary>
			/// <param name="v1">ベクトル</param>
			/// <param name="v2">最大ベクトル</param>
			/// <returns>ベクトル</returns>
			static Vector3 Max(const Vector3& v1, const Vector3& v2);

			/// <summary>
			/// std::min()の3次元ベクトル版
			/// </summary>
			/// <param name="v1">ベクトル</param>
			/// <param name="v2">最小ベクトル</param>
			/// <returns>ベクトル</returns>
			static Vector3 Min(const Vector3& v1, const Vector3& v2);

			Vector3 operator+();
			Vector3 operator-();
			Vector3& operator+=(const Vector3& v);
			Vector3& operator-=(const Vector3& v);
			Vector3& operator*=(float s);
			Vector3& operator*=(const Vector3& v);
			Vector3& operator/=(float s);
		};
		const Vector3 operator+(const Vector3& v1, const Vector3& v2);
		const Vector3 operator-(const Vector3& v1, const Vector3& v2);
		const Vector3 operator*(const Vector3& v, float s);
		const Vector3 operator*(float s, const Vector3& v);
		const Vector3 operator*(const Vector3& v1, const Vector3& v2);
		const Vector3 operator/(const Vector3& v, float s);
		const Vector3 operator/(float s, const Vector3& v);
	}
}
