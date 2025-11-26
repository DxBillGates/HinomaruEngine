#pragma once
namespace GE
{
	namespace Math
	{
		struct Vector2
		{
			float x;
			float y;

			/// <summary>
			/// ゼロ初期化のコンストラクタ
			/// </summary>
			Vector2();

			/// <summary>
			/// x,yをxyで初期化するコンストラクタ
			/// </summary>
			/// <param name="xy">値</param>
			Vector2(float xy);

			/// <summary>
			/// x,yをx,yで初期化するコンストラクタ
			/// </summary>
			/// <param name="x">値</param>
			/// <param name="y">値</param>
			Vector2(float x, float y);


			/// <summary>
			/// このインスタンスの長さを返す関数
			/// </summary>
			/// <returns>長さ</returns>
			float Length();

			/// <summary>
			/// このインスタンスの正規化ベクトルを返す関数
			/// </summary>
			/// <returns>正規化ベクトル</returns>
			Vector2 Normalize();

			/// <summary>
			/// 2つのベクトルからなるベクトルの長さを返す関数
			/// </summary>
			/// <param name="v1">ベクトル</param>
			/// <param name="v2">ベクトル</param>
			/// <returns>長さ</returns>
			static float Distance(const Vector2& v1, const Vector2& v2);

			/// <summary>
			/// 正規化ベクトルを返す関数
			/// </summary>
			/// <param name="v">ベクトル</param>
			/// <returns>正規化ベクトル</returns>
			static Vector2 Normalize(const Vector2& v);

			/// <summary>
			/// 2つのベクトルの外積を返す関数
			/// </summary>
			/// <param name="v1">ベクトル</param>
			/// <param name="v2">ベクトル</param>
			/// <returns>外積</returns>
			static float Cross(const Vector2& v1, const Vector2& v2);

			/// <summary>
			/// 2つのベクトルの内積を返す関数
			/// </summary>
			/// <param name="v1">ベクトル</param>
			/// <param name="v2">ベクトル</param>
			/// <returns>内積</returns>
			static float Dot(const Vector2& v1, const Vector2& v2);

			/// <summary>
			/// 2つのベクトルの線形補間を返す関数
			/// </summary>
			/// <param name="s">スタート位置</param>
			/// <param name="e">終了位置</param>
			/// <param name="t">タイム 0 ~ 1</param>
			/// <returns>線形補間結果</returns>
			static Vector2 Lerp(const Vector2& s, const Vector2& e, float t);

			Vector2 operator+();
			Vector2 operator-();
			Vector2& operator+=(const Vector2& v);
			Vector2& operator-=(const Vector2& v);
			Vector2& operator*=(float s);
			Vector2& operator/=(float s);
		};
		const Vector2 operator+(const Vector2& v1, const Vector2& v2);
		const Vector2 operator-(const Vector2& v1, const Vector2& v2);
		const Vector2 operator*(const Vector2& v, float s);
		const Vector2 operator*(float s, const Vector2& v);
		const Vector2 operator/(const Vector2& v, float s);
		const Vector2 operator/(float s, const Vector2& v);
	}
}