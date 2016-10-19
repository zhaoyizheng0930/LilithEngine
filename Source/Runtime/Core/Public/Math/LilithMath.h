#pragma once

#include "HAL/PlatformIncludes.h"


class FMath :public FPlatformMath
{
public:
	template<class T>
	static int Clamp(const T value, const T max, const T min)
	{
		return value<min ? min : value<max ? value : max;
	}
protected:
private:
};

class FColor
{
public:
	union { struct { uint8 B, G, R, A; }; uint32 AlignmentDummy; };

	FColor(int32 color)
	{
		*((int32*)this) = color;
	}
protected:
private:
};
