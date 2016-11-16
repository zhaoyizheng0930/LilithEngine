#pragma once
#include "RenderResource.h"

class FD3D11ConstantBuffer : public FRenderResource
{
public:
	FD3D11ConstantBuffer(FD3D11DynamicRHI* InD3DRHI, uint32 InSize = 0, uint32 SubBuffers = 1):
		D3DRHI(InD3DRHI),
		MaxSize(InSize),
		CurrentUpdateSize(0)
	{
		InitResource();
	}

	virtual ~FD3D11ConstantBuffer()
	{

	}

	void InitDynamicRHI()
	{
		ShadowData = (uint8*)FMemory::Malloc(MaxSize, 16);
		FMemory::Memzero(ShadowData, MaxSize);
	}

	void ReleaseDynamicRHI()
	{
		if (ShadowData)
		{
			FMemory::Free(ShadowData);
		}
	}

	void UpdateConstant(const uint8* Data , uint16 Offset , uint16 InSize)
	{
		FMemory::Memcpy(ShadowData + Offset, Data, InSize);
		CurrentUpdateSize = FMath::Max((uint32)(Offset + InSize), CurrentUpdateSize);
	}
protected:
	FD3D11DynamicRHI* D3DRHI;
	uint32 MaxSize;
	uint8* ShadowData;

	uint32 CurrentUpdateSize;
private:
};