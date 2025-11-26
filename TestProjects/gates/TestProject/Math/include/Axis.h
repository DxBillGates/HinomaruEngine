#pragma once
#include "Vector3.h"
namespace GE
{
	namespace Math
	{
		struct Axis
		{
			union
			{
				struct
				{
					Vector3 x;
					Vector3 y;
					Vector3 z;
				};
				Vector3 value[3];
			};

			Axis()
				: x({1,0,0})
				, y({0,1,0})
				, z({0,0,1})
			{
			}
		};
	}
}