#pragma once
#include "RunnableThread.h"

class FFakeThread :public FRunnableThread
{
public:
	virtual void SetThreadPriority(EThreadPriority NewPriority)
	{

	}

	virtual void Suspend(bool bShouldPause = true)
	{

	}

	virtual bool Kill(bool bShouldWait = true)
	{

	}

	virtual void WaitForCompletion()
	{

	}
protected:
	virtual bool CreateInternal(FRunnable* InRunnable, const TCHAR* InThreadName,
		uint32 InStackSize = 0,
		EThreadPriority InThreadPri = TPri_Normal, uint64 InThreadAffinityMask = 0)
	{
		return false;
	}
private:
};