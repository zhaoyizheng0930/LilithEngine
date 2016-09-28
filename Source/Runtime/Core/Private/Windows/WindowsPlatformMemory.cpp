#include "../../Public/Windows/WindowsPlatformMemory.h"


class FMalloc* FWindowsPlatformMemory::BaseAllocator()
{
	return new FMallocTBB();
}