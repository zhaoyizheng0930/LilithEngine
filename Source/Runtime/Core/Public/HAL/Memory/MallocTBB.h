#pragma once
#include "MemoryBase.h"

class FMallocTBB:public FMalloc
{
public:
	FMallocTBB()
	{

	}


public:
	virtual void* Malloc(SIZE_T Size, uint32 Alignment) override;
	virtual void* Realloc(void* Ptr, SIZE_T NewSize, uint32 Alignment) override;
	virtual void Free(void* Ptr) override;
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