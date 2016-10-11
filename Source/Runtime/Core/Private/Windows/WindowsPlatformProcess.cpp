#include "CorePCH.h"
#include "../../Public/Windows/WindowsPlatformProcess.h"
#include "../../Public/HAL/Thread/EventPool.h"


void FWindowsPlatformProcess::Sleep(float Seconds)
{
	::Sleep((uint32)(Seconds * 1000.0));
}

FEvent* FWindowsPlatformProcess::GetSynchEventFromPool(bool bIsManualReset)
{
	return bIsManualReset
		? FEventPool<EEventPoolTypes::ManualReset>::Get().GetEventFromPool()
		: FEventPool<EEventPoolTypes::AutoReset>::Get().GetEventFromPool();
}

void FWindowsPlatformProcess::ReturnSynchEventToPool(FEvent* Event)
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

FRunnableThread* FWindowsPlatformProcess::CreateRunnableThread()
{
	return new FRunnableThreadWin();
}

void FWindowsPlatformProcess::SetThreadAffinityMask(uint64 AffinityMask)
{
	//if (AffinityMask != FPlatformAffinity::GetNoAffinityMask())
	//{
	::SetThreadAffinityMask(::GetCurrentThread(), (DWORD_PTR)AffinityMask);
	//}
}

FEvent* FWindowsPlatformProcess::CreateSynchEvent(bool bIsManualReset)
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

	if (!Event->Create(bIsManualReset))
	{
		delete Event;
		Event = NULL;
	}

	return Event;
}

