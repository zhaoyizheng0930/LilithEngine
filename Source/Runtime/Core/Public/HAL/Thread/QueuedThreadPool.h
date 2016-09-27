#pragma once
#include "QueuedWork.h"
#include "QueuedThread.h"

class FQueuedThreadPool
{
public:

	virtual bool Create(uint32 InNumQueuedThreads, uint32 StackSize = (32 * 1024), EThreadPriority ThreadPriority = TPri_Normal) = 0;

	virtual void Destroy() = 0;

	virtual void AddQueuedWork(IQueuedWork* InQueuedWork) = 0;

	virtual bool RetractQueuedWork(IQueuedWork* InQueuedWork) = 0;

	virtual IQueuedWork* ReturnToPoolOrGetNextJob(class FQueuedThread* InQueuedThread) = 0;

public:
	virtual ~FQueuedThreadPool() { }

public:
	static FQueuedThreadPool* Allocate();

protected:
private:
};