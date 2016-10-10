#include "../../CorePrivatePCH.h"
#include "../../../Public/HAL/Thread/WindowsRunnableThread.h"
#include "../../../Public/HAL/Thread/ThreadManager.h"

FRunnableThreadWin::FRunnableThreadWin()
{

}

FRunnableThreadWin::~FRunnableThreadWin()
{

}

void FRunnableThreadWin::SetThreadPriority(EThreadPriority NewPriority)
{
	if (NewPriority != ThreadPriority)
	{
		ThreadPriority = NewPriority;
		// Change the priority on the thread
		::SetThreadPriority(Thread, TranslateThreadPriority(ThreadPriority));
	}
}

void FRunnableThreadWin::Suspend(bool bShouldPause)
{

}

bool FRunnableThreadWin::Kill(bool bShouldWait)
{
	return true;
}

void FRunnableThreadWin::WaitForCompletion()
{

}

uint32 FRunnableThreadWin::GuardedRun()
{
	FPlatformProcess::SetThreadAffinityMask(ThreadAffinityMask);

	return Run();
}

uint32 FRunnableThreadWin::Run()
{
	uint32 ExitCode = 1;
	if (Runnable->Init())
	{
		ThreadInitSyncEvent->Trigger();

		SetTls();

		ExitCode = Runnable->Run();

		Runnable->Exit();

		FreeTls();
	}
	else
	{
		ThreadInitSyncEvent->Trigger();
	}
	return ExitCode;
}

bool FRunnableThreadWin::CreateInternal(FRunnable* InRunnable, const TCHAR* InThreadName,
	uint32 InStackSize,
	EThreadPriority InThreadPri, uint64 InThreadAffinityMask)
{
	Runnable = InRunnable;

	ThreadAffinityMask = InThreadAffinityMask;

	ThreadInitSyncEvent = FPlatformProcess::GetSynchEventFromPool(true);

	Thread = CreateThread(NULL, InStackSize, _ThreadProc, this, STACK_SIZE_PARAM_IS_A_RESERVATION, (::DWORD *)&ThreadID);

	if (Thread == nullptr)
	{
		Runnable = nullptr;
	}
	else
	{
		//FThreadManager::Get().AddThread(Thread , this);

		//ThreadStartUp
		ThreadInitSyncEvent->Wait(INFINITE);
		ThreadName = InThreadName ? InThreadName : TEXT("Unnamed");
		SetThreadName(ThreadID, ThreadName.c_str());
		SetThreadPriority(InThreadPri);
	}

	FPlatformProcess::ReturnSynchEventToPool(ThreadInitSyncEvent);
	ThreadInitSyncEvent = nullptr;
	return Thread != NULL;
}