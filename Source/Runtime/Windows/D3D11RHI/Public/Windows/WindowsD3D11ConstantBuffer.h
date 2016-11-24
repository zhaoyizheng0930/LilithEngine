#pragma once

class FWinD3D11ConstantBuffer:public FD3D11ConstantBuffer
{
public:
	FWinD3D11ConstantBuffer(FD3D11DynamicRHI* InD3DRHI, uint32 InSize = 0, uint32 SubBuffers = 1)
		:FD3D11ConstantBuffer(InD3DRHI , InSize , SubBuffers)
		, Buffers(nullptr)
		, CurrentSubBuffer(0)
		, NumSubBuffer(0)
	{

	}

	virtual void	InitDynamicRHI() override;
	virtual void	ReleaseDynamicRHI() override;

	ID3D11Buffer* GetCurrentConstantBuffer()
	{
		return Buffers[CurrentSubBuffer];
	}

	bool CommitConstantsToDevice(bool bDiscardSharedConstants);

	uint32 GetBufferIndex(uint32 Size);
protected:
private:
	ID3D11Buffer** Buffers;
	uint32 CurrentSubBuffer;
	uint32 NumSubBuffer;
};

template <EShaderFrequency ShaderFrequency>
static void CommitConstants(FD3D11ConstantBuffer*InConstantBuffer, FD3D11StateCache* StateCache, uint32 Index, bool bDiscardSharedConstants)
{
	FWinD3D11ConstantBuffer* WinD11ConstantBuffer = (FWinD3D11ConstantBuffer*)InConstantBuffer;

	if (WinD11ConstantBuffer && WinD11ConstantBuffer->CommitConstantsToDevice(bDiscardSharedConstants))
	{
		ID3D11Buffer* CBuffer= WinD11ConstantBuffer->GetCurrentConstantBuffer();
		StateCache->SetConstantBuffer<ShaderFrequency>(CBuffer, Index);
	}
}