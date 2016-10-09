//TODO_SUPPORT windows process multithread later
#pragma once
#include "../GenericPlatform/GenericPlatformProcess.h"
#include "WindowsCriticalSection.h"
#include "../HAL/Thread/WindowsRunnableThread.h"
#include "../HAL/Thread/Event.h"
#include "../HAL/Thread/WindowsEvent.h"
#include "../HAL/Thread/SingleThreadEvent.h"
#include "../HAL/Thread/EventPool.h"

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
		return bIsManualReset
			? FEventPool<EEventPoolTypes::ManualReset>::Get().GetEventFromPool()
			: FEventPool<EEventPoolTypes::AutoReset>::Get().GetEventFromPool();
	}

	static void ReturnSynchEventToPool(FEvent* Event)
	{
		if (!Event)
		{
			return;
		}

		if (Event->IsManualReset())
		{
			FEventPool<EEventPoolTypes::ManualReset>::Get().ReturnToPool(Event);
		}
		else
		{
			FEventPool<EEventPoolTypes::AutoReset>::Get().ReturnToPool(Event);
		}
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