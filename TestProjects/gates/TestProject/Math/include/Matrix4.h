#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Axis.h"
namespace GE
{
	namespace Math
	{
		//4*4行列
		struct Matrix4x4
		{
			union
			{
				struct
				{
					float _11, _12, _13, _14;
					float _21, _22, _23, _24;
					float _31, _32, _33, _34;
					float _41, _42, _43, _44;
				};
				float m[4][4];
			};

			/// <summary>
			/// コンストラクタ
			/// </summary>
			Matrix4x4();
			/// <summary>
			/// 初期化コンストラクタ
			/// </summary>
			/// <param name="m00">m00に対してセットする値</param>
			/// <param name="m01">m01に対してセットする値</param>
			/// <param name="m02">m02に対してセットする値</param>
			/// <param name="m03">m03に対してセットする値</param>
			/// <param name="m10">m10に対してセットする値</param>
			/// <param name="m11">m11に対してセットする値</param>
			/// <param name="m12">m12に対してセットする値</param>
			/// <param name="m13">m13に対してセットする値</param>
			/// <param name="m20">m20に対してセットする値</param>
			/// <param name="m21">m21に対してセットする値</param>
			/// <param name="m22">m22に対してセットする値</param>
			/// <param name="m23">m23に対してセットする値</param>
			/// <param name="m30">m30に対してセットする値</param>
			/// <param name="m31">m31に対してセットする値</param>
			/// <param name="m32">m32に対してセットする値</param>
			/// <param name="m33">m33に対してセットする値</param>
			Matrix4x4
			(
				float m00, float m01, float m02, float m03,
				float m10, float m11, float m12, float m13,
				float m20, float m21, float m22, float m23,
				float m30, float m31, float m32, float m33
			);

			/// <summary>
			/// 単位行列を返す関数
			/// </summary>
			/// <returns>単位行列</returns>
			static Matrix4x4 Identity();

			/// <summary>
			/// 行列の各軸を返す関数
			/// </summary>
			/// <param name="m">行列</param>
			/// <returns>各軸</returns>
			static Axis GetAxis(const Matrix4x4& m);

			/// <summary>
			/// 大きさ行列を生成する関数
			/// </summary>
			/// <param name="v">大きさ</param>
			/// <returns>大きさ行列</returns>
			static Matrix4x4 Scale(const Vector3& v);

			/// <summary>
			/// X軸での回転行列を生成する関数
			/// </summary>
			/// <param name="angle">ラジアン</param>
			/// <returns>X軸回転行列</returns>
			static Matrix4x4 RotationX(float angle);

			/// <summary>
			/// Y軸での回転行列を生成する関数
			/// </summary>
			/// <param name="angle">ラジアン</param>
			/// <returns>Y軸回転行列</returns>
			static Matrix4x4 RotationY(float angle);

			/// <summary>
			/// Z軸での回転行列を生成する関数
			/// </summary>
			/// <param name="angle">ラジアン</param>
			/// <returns>Z軸回転行列</returns>
			static Matrix4x4 RotationZ(float angle);

			/// <summary>
			/// ZXYの順で回転行列を生成する関数
			/// </summary>
			/// <param name="angles">度数法</param>
			/// <returns>ZXY回転行列</returns>
			static Matrix4x4 RotationZXY(const Vector3& angles);

			/// <summary>
			/// 平行移動行列を生成する関数
			/// </summary>
			/// <param name="v">位置</param>
			/// <returns>平行移動行列</returns>
			static Matrix4x4 Translate(const Vector3& v);

			/// <summary>
			/// ベクトルと行列から新たなベクトルを生成する関数
			/// </summary>
			/// <param name="v">ベクトル</param>
			/// <param name="m">行列</param>
			/// <returns>生成されたベクトル</returns>
			static Vector3 Transform(const Vector3& v, const Matrix4x4& m);

			/// <summary>
			/// 行列の転置を返す関数
			/// </summary>
			/// <param name="m">行列</param>
			/// <returns>転置行列</returns>
			static Matrix4x4 Transpose(const Matrix4x4& m);

			/// <summary>
			/// 逆行列を返す関数
			/// </summary>
			/// <param name="m">行列</param>
			/// <returns>逆行列</returns>
			static Matrix4x4 Inverse(const Matrix4x4& m);

			/// <summary>
			/// 行列式を生成する関数
			/// </summary>
			/// <param name="m">行列</param>
			/// <returns>行列式</returns>
			static float Determinant(const Matrix4x4& m);

			/// <summary>
			/// このインスタンスの各軸を返す
			/// </summary>
			/// <returns></returns>
			Axis GetAxis() const;

			Math::Vector3 GetPosition();

			/// <summary>
			/// 位置、視点位置、上ベクトルを指定してビュー行列を生成する関数
			/// </summary>
			/// <param name="pos">位置</param>
			/// <param name="focasPos">視点位置</param>
			/// <param name="upDir">上ベクトル</param>
			/// <returns>ビュー行列</returns>
			static Matrix4x4 GetViewMatrixLookAt(const Vector3& pos, const Vector3& focasPos, const Vector3& upDir);

			/// <summary>
			/// 位置、方向、上ベクトルを指定してビュー行列を生成する関数
			/// </summary>
			/// <param name="pos">位置</param>
			/// <param name="direction">方向</param>
			/// <param name="upDir">上ベクトル</param>
			/// <returns>ビュー行列</returns>
			static Matrix4x4 GetViewMatrixLookTo(const Vector3& pos, const Vector3& direction, const Vector3& upDir);

			/// <summary>
			/// 透視投影行列を生成する関数
			/// </summary>
			/// <param name="fov">視野角(横ではなく縦)</param>
			/// <param name="aspect">アスペクト比</param>
			/// <param name="nearClip">ニア―クリップ</param>
			/// <param name="farClip">ファークリップ</param>
			/// <returns>透視投影行列</returns>
			static Matrix4x4 GetPerspectiveMatrix(float fov, float aspect, float nearClip = 1.0f, float farClip = 10000.0f);

			/// <summary>
			/// 平行投影行列を生成する関数
			/// </summary>
			/// <param name="size">サイズ</param>
			/// <returns>平行投影行列</returns>
			static Matrix4x4 GetOrthographMatrix(const Vector2& size);

			/// <summary>
			/// 平行投影行列(奥行きがあるオブジェクト対応)を生成する関数
			/// </summary>
			/// <param name="size">サイズ</param>
			/// <param name="nearClip">ニア―クリップ</param>
			/// <param name="farClip">ファークリップ</param>
			/// <returns></returns>
			static Matrix4x4 GetOrthographMatrix(const Vector2& size, float nearClip, float farClip);
		};
		Matrix4x4& operator*=(Matrix4x4& m1, const Matrix4x4& m2);
		Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);
		Vector3 operator*(const Vector3& v, const Matrix4x4& m);
	}
}
