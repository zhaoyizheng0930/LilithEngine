#include "../CorePrivatePCH.h"
#include "../../Public/Windows/WindowsPlatformMemory.h"
#include "../../Public/HAL/Memory/MallocTBB.h"


class FMalloc* FWindowsPlatformMemory::BaseAllocator()
{
	return new FMallocTBB();
}