#include "../../../Public/HAL/Thread/RunnableThread.h"


uint32 FRunnableThread::GetTlsSlot()
{

}

FRunnableThread* FRunnableThread::Create(class FRunnable* InRunnable, const TCHAR* ThreadName, bool bAutoDeleteSelf,
	bool bAutoDeleteRunnable = false,uint32 InStackSize = 0, EThreadPriority InThreadPri = TPri_Normal,
	uint64 InThreadAffinityMask = 0)
{

}

FRunnableThread* FRunnableThread::Create(class FRunnable* InRunnable, const TCHAR* ThreadName, uint32 InStackSize = 0,
	EThreadPriority InThreadPri = TPri_Normal, uint64 InThreadAffinityMask = FPlatformAffinity::GetNoAffinityMask())
{

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