#include "CorePCH.h"
#include "../../../Public/HAL/Thread/QueuedThread.h"

int FQueuedThread::ThreadIndex = 0;

bool FQueuedThread::Create(class FQueuedThreadPool* InPool, uint32 InStackSize, EThreadPriority ThreadPriority)
{
	OwningThreadPool = InPool;
	std::stringstream ss;
	ss << ThreadIndex;
	ThreadName = "PoolThread " + ss.str();
	ThreadIndex++;

	DoWorkEvent = FPlatformProcess::GetSynchEventFromPool();
	Thread = FRunnableThread::Create(this, ThreadName.c_str(), InStackSize, ThreadPriority);
	return true;
}

bool FQueuedThread::KillThread()
{
	DoWorkEvent->Trigger();

	Thread->WaitForCompletion();

	FPlatformProcess::ReturnSynchEventToPool(DoWorkEvent);

	DoWorkEvent = nullptr;
	delete Thread;
	return true;
}

void FQueuedThread::DoWork(IQueuedWork* InQueuedWork)
{
	QueuedWork = InQueuedWork;

	DoWorkEvent->Trigger();
}

uint32 FQueuedThread::Run()
{
	bool bContinueWaiting = true;
	while (bContinueWaiting)
	{
		// Wait for some work to do
		bContinueWaiting = !DoWorkEvent->Wait(10);
	}

	IQueuedWork* LocalQueuedWork = QueuedWork;
	QueuedWork = nullptr;

	while (LocalQueuedWork)
	{
		// Tell the object to do the work
		LocalQueuedWork->DoThreadedWork();
		// Let the object cleanup before we remove our ref to it
		LocalQueuedWork = OwningThreadPool->ReturnToPoolOrGetNextJob(this);
	}

	return 0;
}