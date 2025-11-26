#pragma once
namespace GE
{
	namespace Math
	{
		//イージングチートシート
		class Easing
		{
		public:
			static float Lerp(float s, float e, float t);
			static float EaseInSine(float t);
			static float EaseOutSine(float t);
			static float EaseInOutSine(float t);
			static float EaseInQuad(float t);
			static float EaseOutQuad(float t);
			static float EaseInOutQuad(float x);
			static float EaseInCubic(float x);
			static float EaseOutCubic(float x);
			static float EaseInOutCubic(float x);
			static float EaseInQuart(float x);
			static float EaseOutQuart(float x);
			static float EaseInOutQuart(float x);
			static float EaseInQuint(float x);
			static float EaseOutQuint(float x);
			static float EaseInOutQuint(float x);
			static float EaseInExpo(float x);
			static float EaseOutExpo(float x);
			static float EaseInOutExpo(float x);
			static float EaseInCirc(float x);
			static float EaseOutCirc(float x);
			static float EaseInOutCirc(float x);
			static float EaseInBack(float x);
			static float EaseOutBack(float x);
			static float EaseInOutBack(float x);
			static float EaseInElastic(float x);
			static float EaseOutElastic(float x);
			static float EaseInOutElastic(float x);
			static float EaseInBounce(float x);
			static float EaseOutBounce(float x);
			static float EaseInOutBounce(float x);
		};
	}
}