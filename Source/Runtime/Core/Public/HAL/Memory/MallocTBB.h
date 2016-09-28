#pragma once
#include "MemoryBase.h"

class FMallocTBB:public FMalloc
{
public:
	FMallocTBB()
	{

	}


public:
	virtual void* Malloc(SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT) override;

	virtual void* Realloc(void* Original, SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT) override;

	virtual void Free(void* Original) override;

	virtual bool GetAllocationSize(void *Original, SIZE_T &SizeOut) override;

	virtual const TCHAR* GetDescriptiveName() override
	{
		return TEXT("TBB");
	}
protected:

	void OutOfMemory(uint64 Size, uint32 Alignment)
	{
		// this is expected not to return
		FPlatformMemory::OnOutOfMemory(Size, Alignment);
	}
private:
};