#pragma once
#include "Event.h"
#include "Runnable.h"
#include "RunnableThread.h"

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

	virtual bool Create(class FQueuedThreadPool* InPool, uint32 InStackSize = 0, EThreadPriority ThreadPriority = TPri_Normal)
	{

	}

	virtual bool KillThread()
	{

	}

	void DoWork(IQueuedWork* InQueuedWork)
	{

	}

protected:
	FEvent* DoWorkEvent;

	IQueuedWork* volatile QueuedWork;

	class FQueuedThreadPool* OwningThreadPool;

	//MyThread
	FRunnableThread* Thread;

	virtual uint32 Run() override
	{

	}

private:
};