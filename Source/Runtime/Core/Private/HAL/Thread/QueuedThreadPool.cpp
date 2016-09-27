#include "../../../Public/HAL/Thread/QueuedThreadPool.h"


FQueuedThreadPool* FQueuedThreadPool::Allocate()
{
	return new FQueuedThreadPoolBase;
}