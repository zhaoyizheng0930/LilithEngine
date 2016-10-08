#pragma once
#include "../Core.h"

#include "Thread/QueuedThreadPool.h"
#include "PlatformIncludes.h"

extern FQueuedThreadPool* GThreadPool;

/**
* Implementation of a queued thread pool.
*/
class FQueuedThreadPoolBase : public FQueuedThreadPool
{
protected:
	std::vector<IQueuedWork*> QueuedWork;

	std::vector<FQueuedThread*> QueuedWork;

	std::vector<FQueuedThread*> QueuedWork;

	FCriticalSection* SynchQueue;

public:
	FQueuedThreadPoolBase()
		:SynchQueue(nullptr)
	{

	}

	virtual ~FQueuedThreadPoolBase()
	{
		Destroy();
	}

	virtual bool Create(uint32 InNumQueuedThreads, uint32 StackSize = (32 * 1024), EThreadPriority ThreadPriority = TPri_Normal) override
	{

	}

	virtual void Destroy() override
	{

	}

	int32 GetNumQueuedJobs()
	{

	}

	void AddQueuedWork(IQueuedWork* InQueuedWork) override
	{

	}

	virtual bool RetractQueuedWork(IQueuedWork* InQueuedWork) override
	{

	}

	virtual IQueuedWork* ReturnToPoolOrGetNextJob(FQueuedThread* InQueuedThread) override
	{

	}
}

