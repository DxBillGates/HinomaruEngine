#include "../include/Math.h"
#include <cmath>

float GE::Math::ConvertToRadian(float value)
{
	if (value == 0)return 0;
	float result = PI / 180.0f * value;
	return result;
}

float GE::Math::Lerp(float s, float e, float t)
{
	if (t >= 1)t = 1;
	return s * (1.0f - t) + e * t;
}

float GE::Math::Clamp(float x, float min, float max)
{
	float result = std::fminf(std::fmax(x, min), max);
	return result;
}

float GE::Math::GaussFilter(const GE::Math::Vector2& pos, float value)
{
	return std::expf(-(pos.x * pos.x + pos.y * pos.y) / (2.0f * value * value));
}

void GE::Math::SetGaussFilterData(const GE::Math::Vector2& size, GE::Math::Vector2& dir, float gaussValue, GaussFilterData* data, int dataSize)
{
	GE::Math::Vector2 perPixel = { 1.0f / size.x,1.0f / size.y };

	data->data[0] = GE::Math::Vector4();
	data->data[0].z = GaussFilter({ 0,0 }, gaussValue);
	float totalWeight = data[0].data->z;

	for (int i = 1; i < 8; ++i)
	{
		data->data[i].x = dir.x * i * perPixel.x;
		data->data[i].y = dir.y * i * perPixel.y;
		data->data[i].z = GaussFilter(dir * (float)i, gaussValue);
		totalWeight += data->data[i].z * 2;
	}

	for (int i = 0; i < 8; ++i)
	{
		data->data[i].z /= totalWeight;
	}

	// 負の方向の値もセット
	for (int i = 8; i < 15; ++i)
	{
		data->data[i].x = -data->data[i - 7].x;
		data->data[i].y = -data->data[i - 7].y;
		data->data[i].z = data->data[i - 7].z;
	}
}
