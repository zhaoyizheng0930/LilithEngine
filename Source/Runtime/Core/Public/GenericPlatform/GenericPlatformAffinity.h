#pragma once
#include "HAL/Platform.h"

class FGenericPlatformAffinity
{
public:
	static const uint64 GetMainGameMask()
	{
		return 0xFFFFFFFFFFFFFFFF;
	}

	static const uint64 GetRenderingThreadMask()
	{
		return 0xFFFFFFFFFFFFFFFF;
	}

	static const uint64 GetRHIThreadMask()
	{
		return 0xFFFFFFFFFFFFFFFF;
	}

	static const uint64 GetRTHeartBeatMask()
	{
		return 0xFFFFFFFFFFFFFFFF;
	}

	static const uint64 GetPoolThreadMask()
	{
		return 0xFFFFFFFFFFFFFFFF;
	}

	static const uint64 GetTaskGraphThreadMask()
	{
		return 0xFFFFFFFFFFFFFFFF;
	}

	static const uint64 GetStatsThreadMask()
	{
		return 0xFFFFFFFFFFFFFFFF;
	}

	static const uint64 GetAudioThreadMask()
	{
		return 0xFFFFFFFFFFFFFFFF;
	}

	static const uint64 GetNoAffinityMask()
	{
		return 0xFFFFFFFFFFFFFFFF;
	}
protected:
private:
};