#pragma once
#include "RunnableThread.h"

class FRunnableThreadWin :public FRunnableThread
{
public:
	FRunnableThreadWin();
	~FRunnableThreadWin();

	static DWORD __stdcall _ThreadProc(LPVOID pThis)
	{
		return ((FRunnableThreadWin*)pThis)->GuardedRun();
	}

	void SetThreadName(uint32 ThreadID, const char* ThreadName)
	{
		//Support Later
	}

	static int TranslateThreadPriority(EThreadPriority Priority)
	{
		switch (Priority)
		{
		case TPri_AboveNormal: return THREAD_PRIORITY_ABOVE_NORMAL;
		case TPri_Normal: return THREAD_PRIORITY_NORMAL;
		case TPri_BelowNormal: return THREAD_PRIORITY_BELOW_NORMAL;
		case TPri_Highest: return THREAD_PRIORITY_HIGHEST;
		case TPri_Lowest: return THREAD_PRIORITY_LOWEST;
		case TPri_SlightlyBelowNormal: return THREAD_PRIORITY_NORMAL - 1;
		default:  return TPri_Normal;
		}
	}

	virtual void SetThreadPriority(EThreadPriority NewPriority);

	virtual void Suspend(bool bShouldPause = true);

	virtual bool Kill(bool bShouldWait = true);

	virtual void WaitForCompletion();

	uint32 GuardedRun();

	uint32 Run();
protected:
	virtual bool CreateInternal(FRunnable* InRunnable, const TCHAR* InThreadName,
		uint32 InStackSize = 0,
		EThreadPriority InThreadPri = TPri_Normal, uint64 InThreadAffinityMask = 0);
private:
	/** The thread handle for the thread. */
	HANDLE Thread;

	/** The runnable object to execute on this thread. */
	FRunnable* Runnable;

	FEvent* ThreadInitSyncEvent;

	uint64 ThreadAffinityMask;

	uint32 ThreadID;
};