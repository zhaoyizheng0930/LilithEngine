//TODO_SUPPORT windows process multithread later
#pragma once
#include "../GenericPlatform/GenericPlatformProcess.h"
#include "WindowsCriticalSection.h"
#include "../HAL/Thread/WindowsRunnableThread.h"
#include "../HAL/Thread/Event.h"

class FWindowsPlatformProcess
	: public FGenericPlatformProcess
{
public:
	static void Sleep(float Seconds)
	{
		::Sleep((uint32)(Seconds * 1000.0));
	}

	static FEvent* GetSynchEventFromPool(bool bIsManualReset = false)
	{

	}

	static void ReturnSynchEventToPool(FEvent* Event)
	{

	}

	static FRunnableThread* CreateRunnableThread()
	{
		return new FRunnableThreadWin();
	}

	static void SetThreadAffinityMask(uint64 AffinityMask)
	{
		//if (AffinityMask != FPlatformAffinity::GetNoAffinityMask())
		//{
			::SetThreadAffinityMask(::GetCurrentThread(), (DWORD_PTR)AffinityMask);
		//}
	}

	static FEvent* CreateSynchEvent(bool bIsManualReset = false)
	{
		FEvent* Event = nullptr;
		if (FPlatformProcess::SupportsMultithreading())
		{
			Event = new FEventWin();
		}
		else
		{
			Event = new FSingleThreadEvent();
		}

		if (Event->Create(bIsManualReset))
		{
			delete Event;
			Event = NULL;
		}

		return Event;
	}

protected:
private:
};

typedef FWindowsPlatformProcess FPlatformProcess;
typedef FWindowsCriticalSection FCriticalSection;