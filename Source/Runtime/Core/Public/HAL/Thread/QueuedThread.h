#pragma once
#include "Event.h"
#include "Runnable.h"
#include "RunnableThread.h"
#include "../PlatformIncludes.h"
#include "QueuedWork.h"

class FQueuedThread
	: public FRunnable
{
public:
	FQueuedThread()
		: DoWorkEvent(nullptr)
		, QueuedWork(nullptr)
		, OwningThreadPool(nullptr)
		, Thread(nullptr)
	{ } 

	virtual bool Create(class FQueuedThreadPool* InPool, uint32 InStackSize = 0, EThreadPriority ThreadPriority = TPri_Normal);

	virtual bool KillThread();

	void DoWork(IQueuedWork* InQueuedWork);

public:
	static int ThreadIndex;

protected:
	/** The event that tells the thread there is work to do. */
	FEvent* DoWorkEvent;

	IQueuedWork* volatile QueuedWork;

	FQueuedThreadPool* OwningThreadPool;

	//MyThread
	FRunnableThread* Thread;

	virtual uint32 Run() override;

private:

	std::string ThreadName;
};