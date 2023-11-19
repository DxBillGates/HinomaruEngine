#pragma once
#include "CoreTypes.h"

#include <vector>
#include <string>

namespace he
{
	void InitializeProfileData();
	void UpdateProfileData(float deltaTime = DELTATIME60, const float PROFILING_TIME = 30);
}
