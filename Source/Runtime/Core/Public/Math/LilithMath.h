#pragma once

#include "HAL/PlatformIncludes.h"

#include "IntRect.h"

class FMath :public FPlatformMath
{
public:
	template<class T>
	static int Clamp(const T value, const T max, const T min)
	{
		return value<min ? min : value<max ? value : max;
	}

	template<class T>
	static T Max(const T value1, const T value2)
	{
		return value1 < value2 ? value2 : value1;
	}

	template<class T>
	static T Min(const T value1, const T value2)
	{
		return value1 < value2 ? value1 : value2;
	}

	static int32 FloorToInt(float F)
	{
		return _mm_cvt_ss2si(_mm_set_ss(F + F - 0.5f)) >> 1;
	}

	static int32 FloorLog2(uint32 Value)
	{
		uint32 pos = 0;
		if (Value >= 1 << 16) { Value >>= 16; pos += 16; }
		if (Value >= 1 << 8) { Value >>= 8; pos += 8; }
		if (Value >= 1 << 4) { Value >>= 4; pos += 4; }
		if (Value >= 1 << 2) { Value >>= 2; pos += 2; }
		if (Value >= 1 << 1) { pos += 1; }
		return (Value == 0) ? 0 : pos;
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
