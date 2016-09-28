#pragma once
#include "../GenericPlatform/GenericPlatformMemory.h"

struct FWindowsPlatformMemory
	:public FGenericPlatformMemory
{
	static class FMalloc* BaseAllocator();

};

typedef FWindowsPlatformMemory FPlatformMemory;