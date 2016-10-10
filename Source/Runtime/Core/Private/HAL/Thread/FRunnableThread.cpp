#include "CorePrivatePCH.h"
#include "../../../Public/HAL/Thread/RunnableThread.h"
#include "../../../Public/HAL/Thread/FakeThread.h"
#include "../../../Public/HAL/Thread/Runnable.h"


uint32 FRunnableThread::GetTlsSlot()
{
	return 0;
}

FRunnableThread* FRunnableThread::Create(class FRunnable* InRunnable, const TCHAR* ThreadName, bool bAutoDeleteSelf,
	bool bAutoDeleteRunnable,uint32 InStackSize, EThreadPriority InThreadPri,
	uint64 InThreadAffinityMask)
{
	return Create(InRunnable, ThreadName, InStackSize, InThreadPri, InThreadAffinityMask);
}

FRunnableThread* FRunnableThread::Create(class FRunnable* InRunnable, const TCHAR* ThreadName, uint32 InStackSize,
	EThreadPriority InThreadPri, uint64 InThreadAffinityMask)
{
	//call CreateInternal to virtual to windows thread
	FRunnableThread* NewThread = nullptr;
	if (FPlatformProcess::SupportsMultithreading())
	{
		NewThread = FPlatformProcess::CreateRunnableThread();
		if (NewThread)
		{
			if (NewThread->CreateInternal(InRunnable, ThreadName, InStackSize, InThreadPri, InThreadAffinityMask) == false)
			{
				delete NewThread;
				NewThread = nullptr;
			}
		}
	}
	else if(InRunnable->GetSingleThreadInterface())
	{
		NewThread = new FFakeThread();
		if (NewThread->CreateInternal(InRunnable, ThreadName, InStackSize, InThreadPri, InThreadAffinityMask) == false)
		{
			delete NewThread;
			NewThread = nullptr;
		}
	}
	return NewThread;
}

FRunnableThread::FRunnableThread()
{

}

FRunnableThread::~FRunnableThread()
{

}

void FRunnableThread::SetTls()
{

}

void FRunnableThread::FreeTls()
{

}