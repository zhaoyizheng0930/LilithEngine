#include "CorePCH.h"
#include "../../../Public/HAL/ThreadingBase.h"


FQueuedThreadPool* FQueuedThreadPool::Allocate()
{
	return new FQueuedThreadPoolBase;
}