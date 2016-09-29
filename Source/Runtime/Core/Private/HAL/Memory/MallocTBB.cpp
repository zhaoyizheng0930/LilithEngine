#include "../../../Public/HAL/Memory/MallocTBB.h"
#include "tbb/scalable_allocator.h"


void* FMallocTBB::Malloc(SIZE_T Size, uint32 Alignment)
{
	void* NewPtr = NULL;
	if (Alignment == DEFAULT_ALIGNMENT)
	{
		//scalable
		NewPtr = scalable_malloc(Size);
	}
	else
	{
		Alignment = max(Size >= 16? (uint32)16: (uint32)8,Alignment);
		NewPtr = scalable_aligned_malloc(Size, Alignment);
	}

	if (!NewPtr && Size)
	{
		OutOfMemory(Size, Alignment);
	}
}

void* FMallocTBB::Realloc(void* Ptr, SIZE_T NewSize, uint32 Alignment)
{
	void* NewPtr = NULL;
	if (Alignment == DEFAULT_ALIGNMENT)
	{
		NewPtr = scalable_realloc(Ptr , NewSize);
	}
	else
	{
		Alignment = max(NewSize >= 16 ? (uint32)16 : (uint32)8, Alignment);
		NewPtr = scalable_aligned_realloc(Ptr, NewSize , Alignment);
	}

	if (!NewPtr && NewSize)
	{
		OutOfMemory(NewSize, Alignment);
	}
	return NewPtr;
}

void FMallocTBB::Free(void* Ptr)
{
	scalable_free(Ptr);
}

bool FMallocTBB::GetAllocationSize(void *Original, SIZE_T &SizeOut)
{
	SizeOut = scalable_msize(Original);
	return true;
}