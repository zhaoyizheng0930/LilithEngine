#pragma once
#include "Template/TypeHash.h"
#include "HAL/Memory/LilithMemory.h"
#include "RenderResource.h"

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

	bool operator==(const FD3D11LockedKey& Other) const
	{
		return SourceObject == Other.SourceObject && Subresource == Other.Subresource;
	}
	bool operator!=(const FD3D11LockedKey& Other) const
	{
		return SourceObject != Other.SourceObject || Subresource != Other.Subresource;
	}
	bool operator<(const FD3D11LockedKey& Other) const
	{
		return SourceObject > Other.SourceObject ? true : false;
	}

	FD3D11LockedKey& operator=(const FD3D11LockedKey& Other)
	{
		SourceObject = Other.SourceObject;
		Subresource = Other.Subresource;
		return *this;
	}

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

FORCEINLINE uint32 GetD3D11CubeFace(ECubeFace Face)
{
	switch (Face)
	{
	case CubeFace_PosX:
	default:
		return 0;//D3DCUBEMAP_FACE_POSITIVE_X;
	case CubeFace_NegX:
		return 1;//D3DCUBEMAP_FACE_NEGATIVE_X;
	case CubeFace_PosY:
		return 2;//D3DCUBEMAP_FACE_POSITIVE_Y;
	case CubeFace_NegY:
		return 3;//D3DCUBEMAP_FACE_NEGATIVE_Y;
	case CubeFace_PosZ:
		return 4;//D3DCUBEMAP_FACE_POSITIVE_Z;
	case CubeFace_NegZ:
		return 5;//D3DCUBEMAP_FACE_NEGATIVE_Z;
	};
}

class FD3D11DynamicRHI;

class FD3D11DynamicBuffer:public FRenderResource
{
public:
	enum { MAX_BUFFER_SIZES = 4 };

	FD3D11DynamicBuffer(FD3D11DynamicRHI* InD3DRHI,D3D11_BIND_FLAG InBindFlag , uint32* InBufferSizes);

	~FD3D11DynamicBuffer();

	void* Lock(uint32 Size);

	ID3D11Buffer* Unlock();

	virtual void InitRHI() override;

	virtual void ReleaseRHI() override;
protected:
private:
	std::vector<uint32> BufferSize;

	std::vector<ID3D11Buffer*> Buffers;

	FD3D11DynamicRHI* D3DRHI;

	D3D11_BIND_FLAG BindFlag;

	int32 LockedBufferIndex;
};