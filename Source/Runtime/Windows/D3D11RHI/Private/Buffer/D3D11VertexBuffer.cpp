#include "D3D11RHIPCH.h"
#include "../WindowsD3D11DynamicRHI.h"


FRHIVertexBuffer* FD3D11DynamicRHI::RHICreateVertexBuffer(uint32 Size, uint32 InUsage, FRHIResourceCreateInfo& CreateInfo)
{
	FD3D11VertexBuffer* VertexBuffer = nullptr;
	if (Direct3DDevice)
	{
		ID3D11Buffer* newBuffer = nullptr;
		D3D11_BUFFER_DESC desc;
		desc.ByteWidth = Size;
		desc.Usage = (InUsage & BUF_AnyDynamic) ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = (InUsage & BUF_AnyDynamic) ? D3D11_CPU_ACCESS_WRITE : 0;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;


		/*if (InUsage & BUF_UnorderedAccess)
		{
			desc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;

			static bool bRequiresRawView = (GMaxRHIFeatureLevel < ERHIFeatureLevel::SM5);
			if (bRequiresRawView)
			{
				Desc.MiscFlags |= D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
			}
		}*/

		if (InUsage & BUF_ByteAddressBuffer)
		{
			desc.MiscFlags |= D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
		}

		if (InUsage & BUF_StreamOutput)
		{
			desc.BindFlags |= D3D11_BIND_STREAM_OUTPUT;
		}

		if (InUsage & BUF_DrawIndirect)
		{
			desc.MiscFlags |= D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;
		}

		if (InUsage & BUF_ShaderResource)
		{
			desc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
		}

		//ZYZ_TODO:Init Data Add Later

		Direct3DDevice->CreateBuffer(&desc, NULL, &newBuffer);
		VertexBuffer = new FD3D11VertexBuffer(newBuffer , Size , InUsage);
	}
	return VertexBuffer;
}

void* FD3D11DynamicRHI::RHILockVertexBuffer(FRHIVertexBuffer* VertexBuffer, uint32 Offset, uint32 Size, EResourceLockMode LockMode)
{
	D3D11_BUFFER_DESC desc;
	FD3D11VertexBuffer* D11VertexBuffer = (FD3D11VertexBuffer*)VertexBuffer;
	D11VertexBuffer->Resource->GetDesc(&desc);

	FD3D11LockedKey LockedKey(D11VertexBuffer->Resource);
	FD3D11LockedData LockedData;

	if (desc.Usage == D3D11_USAGE_DYNAMIC)
	{
		D3D11_MAPPED_SUBRESOURCE MappedResource;
		Direct3DDeviceIMContext->Map(D11VertexBuffer->Resource, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
		LockedData.SetData(MappedResource.pData);
		LockedData.Pitch = MappedResource.RowPitch;
	}
	else
	{
		if (LockMode == RLM_ReadOnly)
		{
			ID3D11Buffer* NewBuffer = nullptr;
			D3D11_BUFFER_DESC newdesc;
			newdesc.Usage = D3D11_USAGE_STAGING;
			newdesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			newdesc.ByteWidth = desc.ByteWidth;
			newdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;//0
			newdesc.MiscFlags = 0;
			newdesc.StructureByteStride = 0;
			Direct3DDevice->CreateBuffer(&newdesc, NULL, &NewBuffer);
			LockedData.StagingResource = NewBuffer;

			// Copy the contents of the vertex buffer to the staging buffer.
			D3D11_BOX SourceBox;
			SourceBox.left = Offset;
			SourceBox.right = Size;
			SourceBox.top = SourceBox.front = 0;
			SourceBox.bottom = SourceBox.back = 1;
			Direct3DDeviceIMContext->CopySubresourceRegion(NewBuffer, 0, 0, 0, 0, D11VertexBuffer->Resource, 0, &SourceBox);

			D3D11_MAPPED_SUBRESOURCE MappedResource;
			Direct3DDeviceIMContext->Map(NewBuffer, 0, D3D11_MAP_READ, 0, &MappedResource);
			LockedData.SetData(MappedResource.pData);
			LockedData.Pitch = MappedResource.RowPitch;
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

void FD3D11DynamicRHI::RHIUnlockVertexBuffer(FRHIVertexBuffer* VertexBuffer)
{
	FD3D11VertexBuffer* D11VertexBuffer = (FD3D11VertexBuffer*)VertexBuffer;
	D3D11_BUFFER_DESC desc;
	D11VertexBuffer->Resource->GetDesc(&desc);
	FD3D11LockedKey lockedKey(D11VertexBuffer->Resource);
	FD3D11LockedData* LockedData;
	LockedData = &((OutstandingLocks.find(lockedKey))->second);
	if (desc.Usage == D3D11_USAGE_DYNAMIC)
	{
		Direct3DDeviceIMContext->Unmap(D11VertexBuffer->Resource, 0);
	}
	else
	{
		if (LockedData->StagingResource)
		{
			Direct3DDeviceIMContext->Unmap(D11VertexBuffer->Resource , 0);
		}
		else
		{
			Direct3DDeviceIMContext->UpdateSubresource(D11VertexBuffer->Resource,lockedKey.Subresource , NULL , LockedData->GetData() , LockedData->Pitch , 0 );
			LockedData->FreeData();
		}
	}

	OutstandingLocks.erase(lockedKey);
}

void FD3D11DynamicRHI::RHICopyVertexBuffer(FRHIVertexBuffer* SourceBufferRHI, FRHIVertexBuffer* DestBufferRHI)
{
	FD3D11VertexBuffer* SourceBuffer = (FD3D11VertexBuffer*)SourceBufferRHI;
	FD3D11VertexBuffer* DestBuffer = (FD3D11VertexBuffer*)DestBufferRHI;

	D3D11_BUFFER_DESC SrcDesc;
	SourceBuffer->Resource->GetDesc(&SrcDesc);

	D3D11_BUFFER_DESC DestDesc;
	DestBuffer->Resource->GetDesc(&DestDesc);

	if (SrcDesc.ByteWidth == DestDesc.ByteWidth)
	{
		Direct3DDeviceIMContext->CopyResource(DestBuffer->Resource, SourceBuffer->Resource);
	}
}
