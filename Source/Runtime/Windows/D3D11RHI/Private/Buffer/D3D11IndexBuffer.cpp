#include "D3D11RHIPCH.h"
#include "../WindowsD3D11DynamicRHI.h"
#include "D3D11Util.h"


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
	D3D11_BUFFER_DESC desc;
	FD3D11IndexBuffer* D11IndexBuffer = (FD3D11IndexBuffer*)IndexBuffer;
	D11IndexBuffer->Resource->GetDesc(&desc);

	FD3D11LockedData LockedData;
	FD3D11LockedKey LockedKey(D11IndexBuffer->Resource);

	if (desc.Usage == D3D11_USAGE_DYNAMIC)
	{
		D3D11_MAPPED_SUBRESOURCE SubResource;
		Direct3DDeviceIMContext->Map(D11IndexBuffer->Resource, 0, D3D11_MAP_WRITE_DISCARD,0 , &SubResource);
		LockedData.SetData(SubResource.pData);
		LockedData.Pitch = SubResource.RowPitch;
	}
	else
	{
		if (LockMode == RLM_ReadOnly)
		{
			ID3D11Buffer* NewBuffer = nullptr;
			D3D11_BUFFER_DESC newdesc;
			newdesc.ByteWidth = desc.ByteWidth;
			newdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			newdesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			newdesc.Usage = D3D11_USAGE_STAGING;
			newdesc.MiscFlags = 0;
			newdesc.StructureByteStride = 0;
			Direct3DDevice->CreateBuffer(&newdesc, NULL ,&NewBuffer);
			LockedData.StagingResource = NewBuffer;

			Direct3DDeviceIMContext->CopyResource(NewBuffer, D11IndexBuffer->Resource);

			D3D11_MAPPED_SUBRESOURCE MappedResource;
			Direct3DDeviceIMContext->Map(NewBuffer , 0 , D3D11_MAP_READ ,0 , &MappedResource );
			LockedData.SetData(MappedResource.pData);
			LockedData.Pitch = MappedResource.RowPitch;
		}
		else
		{
			LockedData.AllocData(desc.ByteWidth);
			LockedData.Pitch = desc.ByteWidth;
		}
	}

	OutstandingLocks.insert(std::make_pair(LockedKey , LockedData));

	return (void*)((uint8*)LockedData.GetData() + Offset);
}

void FD3D11DynamicRHI::RHIUnlockIndexBuffer(FRHIIndexBuffer* IndexBuffer)
{
	FD3D11IndexBuffer* D11Buffer = (FD3D11IndexBuffer*)IndexBuffer;
	ID3D11Buffer* buffer = D11Buffer->Resource;
	D3D11_BUFFER_DESC desc;
	buffer->GetDesc(&desc);

	FD3D11LockedKey LockedKey(buffer);
	FD3D11LockedData* LockedData = &((OutstandingLocks.find(LockedKey))->second);

	if (desc.Usage == D3D11_USAGE_DYNAMIC)
	{
		Direct3DDeviceIMContext->Unmap(buffer, 0);
	}
	else
	{
		if (LockedData->StagingResource)
		{
			Direct3DDeviceIMContext->Unmap(LockedData->StagingResource, 0);
		}
		else
		{
			Direct3DDeviceIMContext->UpdateSubresource(D11Buffer->Resource, 0, NULL, LockedData->GetData(), LockedData->Pitch, 0);
			LockedData->FreeData();
		}
	}

	OutstandingLocks.erase(LockedKey);
}