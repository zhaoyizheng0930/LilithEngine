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

	static int32 FloorToInt(float F)
	{
		return _mm_cvt_ss2si(_mm_set_ss(F + F - 0.5f)) >> 1;
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
