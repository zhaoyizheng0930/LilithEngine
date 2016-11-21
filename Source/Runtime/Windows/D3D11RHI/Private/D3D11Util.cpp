#include "D3D11RHIPCH.h"
#include "WindowsD3D11DynamicRHI.h"


FD3D11DynamicBuffer::FD3D11DynamicBuffer(FD3D11DynamicRHI* InD3DRHI, D3D11_BIND_FLAG InBindFlag, uint32* InBufferSizes)
	:D3DRHI(InD3DRHI),
	BindFlag(InBindFlag)
{
	//InitBufferSize
	while (BufferSize.size() < MAX_BUFFER_SIZES && *InBufferSizes > 0)
	{
		BufferSize.push_back(*InBufferSizes);
		InBufferSizes++;
	}

	InitResource();
}

FD3D11DynamicBuffer::~FD3D11DynamicBuffer()
{
	ReleaseResource();
}

void* FD3D11DynamicBuffer::Lock(uint32 Size)
{
	//Find a Buffer can used
	bool bfind = false;
	ID3D11Buffer* Buffer = NULL;
	for (int i = 0; i < BufferSize.size();i++)
	{
		if (BufferSize[i] > Size)
		{
			bfind = true;
			Buffer = Buffers[i];
			LockedBufferIndex = i;
			break;
		}
	}

	if (!bfind)
	{
		//CreateBuffer
		D3D11_BUFFER_DESC BufferDesc;
		BufferDesc.BindFlags = BindFlag;
		BufferDesc.ByteWidth = Size;
		BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		BufferDesc.MiscFlags = 0;

		D3DRHI->GetDevice()->CreateBuffer(&BufferDesc, NULL, &Buffer);
		//ZYZ_TODO:Don't know why
		//UpdateBufferStats(Buffers[BufferIndex], false);
		//UpdateBufferStats(Buffer, true);
		LockedBufferIndex = Buffers.size();
		Buffers.push_back(Buffer);
	}
	//Use this buffer
	D3D11_MAPPED_SUBRESOURCE MappedSubresource;
	D3DRHI->GetContext()->Map(Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedSubresource);
	return MappedSubresource.pData;
}

ID3D11Buffer* FD3D11DynamicBuffer::Unlock()
{
	ID3D11Buffer* Buffer = Buffers[LockedBufferIndex];

	D3DRHI->GetContext()->Unmap(Buffer, 0);
	LockedBufferIndex = -1;
	return Buffer;
}

void FD3D11DynamicBuffer::InitRHI()
{
	D3D11_BUFFER_DESC BufferDesc;
	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	BufferDesc.BindFlags = BindFlag;
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	BufferDesc.MiscFlags = 0;

	//Init Buffers
	while (Buffers.size() < BufferSize.size())
	{
		ID3D11Buffer* NewBuffer = NULL;
		BufferDesc.ByteWidth = BufferSize[Buffers.size()];
		D3DRHI->GetDevice()->CreateBuffer(&BufferDesc , NULL , &NewBuffer);
		//ZYZ_TODO:Don't know why
		//UpdateBufferStats
		Buffers.push_back(NewBuffer);
	}
}

void FD3D11DynamicBuffer::ReleaseRHI()
{
	for (int i = 0; i < Buffers.size();i++)
	{
		Buffers[i] = NULL;
		//ZYZ_TODO:Don't know why
		//UpdateBufferStats(Buffers[i], false);
	}
	Buffers.clear();
}