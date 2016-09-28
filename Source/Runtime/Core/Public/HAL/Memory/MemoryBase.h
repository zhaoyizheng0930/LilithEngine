#pragma once
#include "../../Core.h"

enum
{
	// Default allocator alignment. If the default is specified, the allocator applies to engine rules.
	// Blocks >= 16 bytes will be 16-byte-aligned, Blocks < 16 will be 8-byte aligned. If the allocator does
	// not support allocation alignment, the alignment will be ignored.
	DEFAULT_ALIGNMENT = 0,

	// Minimum allocator alignment
	MIN_ALIGNMENT = 8,
};

/** The global memory allocator. */
extern class FMalloc* GMalloc;

class FMalloc
{
public:
	virtual void* Malloc(SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT) = 0;

	virtual void* Realloc(void* Original, SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT) = 0;

	virtual void Free(void* Original) = 0;

	virtual bool GetAllocationSize(void *Original, SIZE_T &SizeOut)
	{
		return false;
	}

	virtual SIZE_T QuantizeSize(SIZE_T Count, uint32 Alignment)
	{
		return Count;
	}

	virtual const TCHAR* GetDescriptiveName()
	{
		return TEXT("Unspecified allocator");
	}
protected:
private:
};