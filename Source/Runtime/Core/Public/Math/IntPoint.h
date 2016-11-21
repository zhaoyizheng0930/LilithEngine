#pragma once
#include "HAL/Platform.h"

struct FIntPoint
{
	int32 X;
	int32 Y;
public:
	FIntPoint(int32 InX , int32 InY)
		:X(InX)
		,Y(InY)
	{

	}
};