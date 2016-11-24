#include "D3D11RHIPCH.h"
#include "Windows/WindowsD3D11ConstantBuffer.h"
#include "../WindowsD3D11DynamicRHI.h"

void FWinD3D11ConstantBuffer::InitDynamicRHI()
{
	//Init Buffers
	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = MaxSize;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	Buffers = new ID3D11Buffer*[NumSubBuffer];
	for (int i = 0; i < NumSubBuffer;i++)
	{
		D3DRHI->GetDevice()->CreateBuffer(&desc , NULL , &Buffers[i]);
		//ZYZ_TODO:UpdateBufferState
		desc.ByteWidth = Align(desc.ByteWidth / 2, 16);
	}

	FD3D11ConstantBuffer::InitDynamicRHI();
}

void FWinD3D11ConstantBuffer::ReleaseDynamicRHI()
{
	if (Buffers)
	{
		for (int i = 0; i < NumSubBuffer; i++)
		{
			//ZYZ_TODO:UpdateBufferState
			SAFE_RELEASE(Buffers[i]);
		}
		delete[] Buffers;
		Buffers = NULL;
	}
}

bool FWinD3D11ConstantBuffer::CommitConstantsToDevice(bool bDiscardSharedConstants)
{
	if (bDiscardSharedConstants)
	{
		TotalUpdateSize = CurrentUpdateSize;
	}
	else
	{
		TotalUpdateSize = FMath::Max(CurrentUpdateSize, TotalUpdateSize);
	}

	uint32 BufferIndex = GetBufferIndex(TotalUpdateSize);
	uint32 BufferSize = MaxSize >> (NumSubBuffer - 1 - BufferIndex);

	ID3D11Buffer* Buffer = Buffers[BufferIndex];

	D3DRHI->GetContext()->UpdateSubresource(Buffer, 0, NULL, ShadowData, BufferSize, BufferSize);
	return true;
}

uint32 FWinD3D11ConstantBuffer::GetBufferIndex(uint32 Size)
{
	uint32 BufferSize = MaxSize;
	uint32 BufferIndex = NumSubBuffer - 1;
	for (; BufferIndex >= 0; BufferIndex--)
	{
		if (BufferSize > Size && BufferSize/2 < Size)
		{
			return BufferIndex;
		}
		else
		{
			BufferSize /= 2;
		}
	}

	return BufferIndex;
}