#include "../../../Public/HAL/Memory/MallocTBB.h"

void* FMallocTBB::Malloc(SIZE_T Count, uint32 Alignment)
{
	if (Alignment == DEFAULT_ALIGNMENT)
	{
		//scalable
	}
	else
	{

	}
}

void* FMallocTBB::Realloc(void* Original, SIZE_T Count, uint32 Alignment)
{

}

void FMallocTBB::Free(void* Original)
{

}

bool FMallocTBB::GetAllocationSize(void *Original, SIZE_T &SizeOut)
{

}