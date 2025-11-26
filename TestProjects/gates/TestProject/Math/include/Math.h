#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Quaternion.h"
#include "Easing.h"

namespace GE
{
	namespace Math
	{
		struct GaussFilterData
		{
			GE::Math::Vector4 data[16];
		};

		const float PI = 3.14159265f;

		/// <summary>
		/// 度数法から弧度法に変換する関数
		/// </summary>
		/// <param name="value">度数</param>
		/// <returns>ラジアン</returns>
		float ConvertToRadian(float value);

		/// <summary>
		/// 線形補間関数
		/// </summary>
		/// <param name="s">スタート</param>
		/// <param name="e">エンド</param>
		/// <param name="t">タイム</param>
		/// <returns>線形補間の値</returns>
		float Lerp(float s, float e, float t);

		/// <summary>
		/// xをmin maxの間で返す
		/// 例) (x = 1000,min = 0,max = 100)のときreturn = 100
		/// </summary>
		/// <param name="x">比較したい値</param>
		/// <param name="min"></param>
		/// <param name="max"></param>
		/// <returns></returns>
		float Clamp(float x, float min, float max);

		// ガウシアンフィルタの重みを計算する関数
		float GaussFilter(const GE::Math::Vector2& pos, float value);
		// 参照ピクセルへの情報とガウシアンフィルタの重みを計算してセットする関数
		void SetGaussFilterData(const GE::Math::Vector2& size, GE::Math::Vector2& dir, float gaussValue, GaussFilterData* data, int dataSize);
	}
}
