#include "D3D11RHIPCH.h"
#include "../WindowsD3D11DynamicRHI.h"


FRHIIndexBuffer* FD3D11DynamicRHI::RHICreateIndexBuffer(uint32 Stride, uint32 Size, uint32 InUsage, FRHIResourceCreateInfo& CreateInfo)
{
	FD3D11IndexBuffer* D11IndexBuffer = nullptr;
	if (Direct3DDevice)
	{
		ID3D11Buffer* newbuffer = nullptr;
		D3D11_BUFFER_DESC desc;
		desc.ByteWidth = Size;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.Usage = (InUsage&BUF_AnyDynamic)?D3D11_USAGE_DYNAMIC:D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = (InUsage&BUF_AnyDynamic) ? D3D11_CPU_ACCESS_WRITE : 0;
		desc.MiscFlags = 0;
		if (InUsage & BUF_UnorderedAccess)
		{
			desc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
		}

		if (InUsage & BUF_DrawIndirect)
		{
			desc.MiscFlags |= D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;
		}

		if (InUsage & BUF_ShaderResource)
		{
			desc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
		}
		//Init Data from ResourceCreateInfo will add later.

		Direct3DDevice->CreateBuffer(&desc ,NULL , &newbuffer);

		D11IndexBuffer = new FD3D11IndexBuffer(newbuffer , Stride , Size , InUsage);
	}
	return D11IndexBuffer;
}

void* FD3D11DynamicRHI::RHILockIndexBuffer(FRHIIndexBuffer* IndexBuffer, uint32 Offset, uint32 Size, EResourceLockMode LockMode)
{

}

void FD3D11DynamicRHI::RHIUnlockIndexBuffer(FRHIIndexBuffer* IndexBuffer)
{

}