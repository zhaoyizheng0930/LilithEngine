#pragma once
#include "../PlatformIncludes.h"

class FThreadManager
{
public:
	FThreadManager& FThreadManager::Get()
	{
		static FThreadManager Singleton;
		return Singleton;
	}

	void AddThread(uint32 ThreadId, class FRunnableThread* Thread)
	{

	}
protected:
private:
};