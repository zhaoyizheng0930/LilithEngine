#pragma once
#include "Template/TypeHash.h"
#include "HAL/Memory/LilithMemory.h"

class FD3D11LockedKey
{
public:
	void* SourceObject;
	uint32 Subresource;
public:
	FD3D11LockedKey():SourceObject(NULL),Subresource(0) {}
	FD3D11LockedKey(ID3D11Buffer* Inbuffer,uint32 InSubresource = 0):SourceObject((void*)Inbuffer),Subresource(InSubresource) {}

	FD3D11LockedKey(ID3D11Texture2D* Inbuffer, uint32 InSubresource = 0) :SourceObject((void*)Inbuffer), Subresource(InSubresource) {}

	FD3D11LockedKey(ID3D11Texture3D* Inbuffer, uint32 InSubresource = 0) :SourceObject((void*)Inbuffer), Subresource(InSubresource) {}

	uint32 GetHash() { return PointerHash(SourceObject); };
protected:
private:
};

class FD3D11LockedData
{
public:
	ID3D11Resource* StagingResource;
	uint32 Pitch;
	uint32 DepthPitch;
	 
	FD3D11LockedData()
	{

	}

	void SetData(void* InData)
	{
		Data = (uint8*)InData;
	}

	uint8* GetData()
	{
		return Data;
	}

	void FreeData()
	{
		FMemory::Free((void*)Data);
		Data = NULL;
	}

	void AllocData(uint32 Size)
	{
		Data = (uint8*)FMemory::Malloc(Size, 16);
		bAllocDataWasUsed = true;
	}

protected:
private:
	uint8* Data;
	bool bAllocDataWasUsed;
};