#include "../include/Math.h"
#include "../include/Easing.h"
#include <cmath>

float GE::Math::Easing::Lerp(float s, float e, float t)
{
	return s * (1.0f - t) + e * t;;
}

float GE::Math::Easing::EaseInSine(float t)
{
	return 1 - std::cosf((t * PI) / 2);
}

float GE::Math::Easing::EaseOutSine(float t)
{
	return std::sinf((t * PI) / 2);
}

float GE::Math::Easing::EaseInOutSine(float t)
{
	return -(std::cosf(PI * t) - 1) / 2;
}

float GE::Math::Easing::EaseInQuad(float t)
{
	return t * t;
}

float GE::Math::Easing::EaseOutQuad(float x)
{
	return 1 - (1 - x) * (1 - x);
}

float GE::Math::Easing::EaseInOutQuad(float x)
{
	return x < 0.5f ? 2 * x * x : 1 - std::powf(-2 * x + 2, 2) / 2;
}

float GE::Math::Easing::EaseInCubic(float x)
{
	return x * x * x;
}

float GE::Math::Easing::EaseOutCubic(float x)
{
	return 1 - std::powf(1 - x, 3);
}

float GE::Math::Easing::EaseInOutCubic(float x)
{
	return x < 0.5f ? 4 * x * x * x : 1 - std::powf(-2 * x + 2, 3) / 2;
}

float GE::Math::Easing::EaseInQuart(float x)
{
	return x * x * x * x;
}

float GE::Math::Easing::EaseOutQuart(float x)
{
	return 1 - std::powf(1 - x, 4);
}

float GE::Math::Easing::EaseInOutQuart(float x)
{
	return x < 0.5f ? 8 * x * x * x * x : 1 - std::powf(-2 * x + 2, 4) / 2;
}

float GE::Math::Easing::EaseInQuint(float x)
{
	return x * x * x * x * x;
}

float GE::Math::Easing::EaseOutQuint(float x)
{
	return 1 - std::powf(1 - x, 5);
}

float GE::Math::Easing::EaseInOutQuint(float x)
{
	return x < 0.5f ? 16 * x * x * x * x * x : 1 - std::powf(-2 * x + 2, 5) / 2;
}

float GE::Math::Easing::EaseInExpo(float x)
{
	return x == 0 ? 0 : std::powf(2, 10 * x - 10);
}

float GE::Math::Easing::EaseOutExpo(float x)
{
	return x == 1 ? 1 : 1 - std::powf(2, -10 * x);
}

float GE::Math::Easing::EaseInOutExpo(float x)
{
	return x == 0 ? 0 : x == 1 ? 1 : x < 0.5f ? std::powf(2, 20 * x - 10) / 2 : (2 - std::powf(2, -20 * x + 10)) / 2;
}

float GE::Math::Easing::EaseInCirc(float x)
{
	return 1 - std::sqrtf(1 - std::powf(x, 2));
}

float GE::Math::Easing::EaseOutCirc(float x)
{
	return std::sqrtf(1 - std::powf(x - 1, 2));
}

float GE::Math::Easing::EaseInOutCirc(float x)
{
	return x < 0.5f ? (1 - std::sqrtf(1 - std::powf(2 * x, 2))) / 2 : (std::sqrtf(1 - std::powf(-2 * x + 2, 2)) + 1) / 2;
}

float GE::Math::Easing::EaseInBack(float x)
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1;

	return c3 * x * x * x - c1 * x * x;
}

float GE::Math::Easing::EaseOutBack(float x)
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1;

	return 1 + c3 * std::powf(x - 1, 3) + c1 * std::powf(x - 1, 2);
}

float GE::Math::Easing::EaseInOutBack(float x)
{
	const float c1 = 1.70158f;
	const float c2 = c1 * 1.525f;

	return x < 0.5 ? (std::powf(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2 : (std::powf(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;
}

float GE::Math::Easing::EaseInElastic(float x)
{
	const float c4 = (2 * PI) / 3;

	return x == 0 ? 0 : x == 1 ? 1 : -std::powf(2, 10 * x - 10) * std::sinf((x * 10 - 10.75f) * c4);
}

float GE::Math::Easing::EaseOutElastic(float x)
{
	const float c4 = (2 * PI) / 3;

	return x == 0
		? 0
		: x == 1
		? 1
		: std::powf(2, -10 * x) * std::sinf((x * 10 - 0.75f) * c4) + 1;
}

float GE::Math::Easing::EaseInOutElastic(float x)
{
	const float c5 = (2 * PI) / 4.5f;

	return x == 0
		? 0
		: x == 1
		? 1
		: x < 0.5
		? -(std::powf(2, 20 * x - 10) * std::sinf((20 * x - 11.125f) * c5)) / 2
		: (std::powf(2, -20 * x + 10) * std::sinf((20 * x - 11.125f) * c5)) / 2 + 1;
}

float GE::Math::Easing::EaseInBounce(float x)
{
	return 1 - EaseOutBounce(1 - x);
}

float GE::Math::Easing::EaseOutBounce(float x)
{
	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (x < 1 / d1) {
		return n1 * x * x;
	}
	else if (x < 2 / d1) {
		return n1 * (x -= 1.5f / d1) * x + 0.75f;
	}
	else if (x < 2.5f / d1) {
		return n1 * (x -= 2.25f / d1) * x + 0.9375f;
	}
	else {
		return n1 * (x -= 2.625f / d1) * x + 0.984375f;
	}
}

float GE::Math::Easing::EaseInOutBounce(float x)
{
	return x < 0.5f
		? (1 - EaseOutBounce(1 - 2 * x)) / 2
		: (1 + EaseOutBounce(2 * x - 1)) / 2;
}
