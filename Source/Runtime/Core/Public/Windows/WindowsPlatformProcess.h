//TODO_SUPPORT windows process multithread later
#pragma once
#include "../GenericPlatform/GenericPlatformProcess.h"
#include "../HAL/Thread/Event.h"
#include "../HAL/Thread/RunnableThread.h"
#include "../HAL/Thread/WindowsEvent.h"
#include "../HAL/Thread/SingleThreadEvent.h"
#include "../HAL/Thread/WindowsRunnableThread.h"


class FWindowsPlatformProcess
	: public FGenericPlatformProcess
{
public:
	static bool SupportsMultithreading()
	{
		return true;
	}
	static void Sleep(float Seconds);

	static FEvent* GetSynchEventFromPool(bool bIsManualReset = false);

	static void ReturnSynchEventToPool(FEvent* Event);

	static FRunnableThread* CreateRunnableThread();

	static void SetThreadAffinityMask(uint64 AffinityMask);

	static FEvent* CreateSynchEvent(bool bIsManualReset = false);

protected:
private:
};

typedef FWindowsPlatformProcess FPlatformProcess;