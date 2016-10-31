#include "D3D11RHIPCH.h"
#include "../WindowsD3D11DynamicRHI.h"
#include "D3D11Util.h"


FRHIStructureBuffer* FD3D11DynamicRHI::RHICreateStructureBuffer(uint32 Stride, uint32 Size, uint32 InUsage, FRHIResourceCreateInfo& CreateInfo)
{
	FD3D11StructureBuffer* D11StructBuffer = nullptr;
	ID3D11Buffer* StructBuffer = nullptr;
	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = Size;
	desc.Usage = (InUsage & BUF_AnyDynamic) ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = (InUsage & BUF_AnyDynamic) ? D3D11_CPU_ACCESS_WRITE : 0;
	if (InUsage & BUF_ShaderResource)
	{
		// Setup bind flags so we can create a view to read from the buffer in a shader.
		desc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
	}

	if (InUsage & BUF_UnorderedAccess)
	{
		// Setup bind flags so we can create a writeable UAV to the buffer
		desc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
	}

	if (InUsage & BUF_StreamOutput)
	{
		desc.BindFlags |= D3D11_BIND_STREAM_OUTPUT;
	}
	desc.MiscFlags = 0;
	if (InUsage & BUF_DrawIndirect)
	{
		desc.MiscFlags |= D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;
	}
	else
	{
		if (InUsage & BUF_ByteAddressBuffer)
		{
			desc.MiscFlags |= D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
		}
		else
		{
			desc.MiscFlags |= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		}
	}

	desc.StructureByteStride = Stride;

	//TODO:StructBufferInitLater
	Direct3DDevice->CreateBuffer(&desc, NULL, &StructBuffer);
	D11StructBuffer = new FD3D11StructureBuffer(StructBuffer , Stride , Size , InUsage);

	return D11StructBuffer;
}

void* FD3D11DynamicRHI::RHILockStructureBuffer(FRHIStructureBuffer* StructureBuffer, uint32 Offset, uint32 Size, EResourceLockMode LockMode)
{
	FD3D11StructureBuffer* D11StructureBuffer = (FD3D11StructureBuffer*)StructureBuffer;
	D3D11_BUFFER_DESC desc;
	D11StructureBuffer->Resource->GetDesc(&desc);
	FD3D11LockedKey LockedKey(D11StructureBuffer->Resource);
	FD3D11LockedData LockedData;
	if (desc.Usage == D3D11_USAGE_DYNAMIC)
	{
		D3D11_MAPPED_SUBRESOURCE MappedResource;
		Direct3DDeviceIMContext->Map(D11StructureBuffer->Resource, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
		LockedData.SetData(MappedResource.pData);
		LockedData.Pitch = MappedResource.RowPitch;
	}
	else
	{
		if (LockMode == RLM_ReadOnly)
		{
			ID3D11Buffer* StagingBuffer;
			D3D11_BUFFER_DESC newdesc;
			newdesc.Usage = D3D11_USAGE_STAGING;
			newdesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			newdesc.ByteWidth = desc.ByteWidth;
			newdesc.BindFlags = 0;
			newdesc.MiscFlags = 0;
			newdesc.StructureByteStride = 0;
			Direct3DDevice->CreateBuffer(&newdesc, NULL, &StagingBuffer);

			Direct3DDeviceIMContext->CopyResource(StagingBuffer, D11StructureBuffer->Resource);

			D3D11_MAPPED_SUBRESOURCE MappedSubresource;
			Direct3DDeviceIMContext->Map(StagingBuffer, 0, D3D11_MAP_READ, 0, &MappedSubresource);
			LockedData.SetData(MappedSubresource.pData);
			LockedData.Pitch = MappedSubresource.RowPitch;
		}
		else
		{
			LockedData.AllocData(desc.ByteWidth);
			LockedData.Pitch = desc.ByteWidth;
		}
	}

	OutstandingLocks.insert(std::make_pair(LockedKey, LockedData));

	return (void*)((uint8*)LockedData.GetData() + Offset);

}

void FD3D11DynamicRHI::RHIUnlockStructureBuffer(FRHIStructureBuffer* StructureBuffer)
{
	FD3D11StructureBuffer* D11StructBuffer = (FD3D11StructureBuffer*)StructureBuffer;
	D3D11_BUFFER_DESC desc;
	D11StructBuffer->Resource->GetDesc(&desc);
	FD3D11LockedKey LockedKey(D11StructBuffer->Resource);
	FD3D11LockedData* LockedData = &((OutstandingLocks.find(LockedKey))->second);
	if (desc.Usage == D3D11_USAGE_DYNAMIC)
	{
		Direct3DDeviceIMContext->Unmap(D11StructBuffer->Resource, 0);
	}
	else
	{
		if (LockedData->StagingResource)
		{
			Direct3DDeviceIMContext->Unmap(LockedData->StagingResource, 0);
		}
		else
		{
			// Copy the contents of the temporary memory buffer allocated for writing into the VB.
			Direct3DDeviceIMContext->UpdateSubresource(D11StructBuffer->Resource, LockedKey.Subresource, NULL, LockedData->GetData(), LockedData->Pitch, 0);

			// Check the copy is finished before freeing...
			Direct3DDeviceIMContext->Flush();

			// Free the temporary memory buffer.
			LockedData->FreeData();

		}
	}

	OutstandingLocks.erase(LockedKey);
}