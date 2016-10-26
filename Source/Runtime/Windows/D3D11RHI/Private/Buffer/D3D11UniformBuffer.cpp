#include "D3D11RHIPCH.h"
#include "../WindowsD3D11DynamicRHI.h"
#include "HAL/Memory/LilithMemory.h"


FRHIUniformBuffer* FD3D11DynamicRHI::RHICreateUniformBuffer(const void* Contents, const FRHIUniformBufferLayout& Layout, EUniformBufferUsage Usage)
{
	FD3D11UniformBuffer* UniformBuffer = nullptr;
	bool bUseBufferPool = false;
	ID3D11Buffer* newBuffer = nullptr;

	if (bUseBufferPool)
	{
		//Support Pool Later
	}
	else
	{
		D3D11_BUFFER_DESC desc;
		desc.ByteWidth = Layout.ConstantBufferSize;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		Direct3DDevice->CreateBuffer(&desc , NULL , &newBuffer);

		//MemInit
		D3D11_MAPPED_SUBRESOURCE MappedResource;
		Direct3DDeviceIMContext->Map(newBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
		FMemory::Memcpy(MappedResource.pData, Contents , Layout.ConstantBufferSize);
		Direct3DDeviceIMContext->Unmap(newBuffer, 0);

	}

	UniformBuffer = new FD3D11UniformBuffer(this , Layout , newBuffer);

	return UniformBuffer;
}