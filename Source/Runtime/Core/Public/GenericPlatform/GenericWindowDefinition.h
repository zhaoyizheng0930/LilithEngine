#pragma once
#include "../HAL/Platform.h"
#include <string>

struct FGenericWindowDefinition
{
	int32 XDesiredPositionOnScreen;

	int32 YDesiredPositionOnScreen;

	int32 WidthDesiredOnScreen;

	int32 HeightDesiredOnScreen;

	std::string Title;
};