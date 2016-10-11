#include "CorePCH.h"
#include "HAL/Memory/LilithMemory.h"
#include "HAL/Memory/FMemory.inl"

FMalloc* GMalloc = nullptr;

void FMemory::GCreateMalloc()
{
	GMalloc = FPlatformMemory::BaseAllocator();
}

void* FMemory::MallocExternal(SIZE_T Count, uint32 Alignment)
{
	if (!GMalloc)
	{
		GCreateMalloc();
	}
	return GMalloc->Malloc(Count, Alignment);
}

void* FMemory::ReallocExternal(void* Original, SIZE_T Count, uint32 Alignment)
{
	if (!GMalloc)
	{
		GCreateMalloc();
	}
	return GMalloc->Realloc(Original, Count, Alignment);
}

void FMemory::FreeExternal(void* Original)
{
	if (!GMalloc)
	{
		GCreateMalloc();
	}
	GMalloc->Free(Original);
}

SIZE_T FMemory::GetAllocSizeExternal(void* Original)
{
	if (!GMalloc)
	{
		GCreateMalloc();
	}

	SIZE_T Size = 0;
	return GMalloc->GetAllocationSize(Original, Size) ? Size : 0;
}

SIZE_T FMemory::QuantizeSizeExternal(SIZE_T Count, uint32 Alignment)
{
	if (!GMalloc)
	{
		GCreateMalloc();
	}
	return GMalloc->QuantizeSize(Count , Alignment);
}