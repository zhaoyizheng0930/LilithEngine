#pragma  once
#include "IntPoint.h"

struct FIntRect
{
	FIntPoint Min;
	FIntPoint Max;

public:
	FIntRect(FIntPoint InMin, FIntPoint InMax)
		:Min(InMin),
		Max(InMax)
	{

	}
};