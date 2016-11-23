#include "D3D11RHIPCH.h"
#include "WindowsD3D11DynamicRHI.h"
#include "RenderUtils.h"

FRHIShaderResourceView* FD3D11DynamicRHI::RHICreateShaderResourceView(FRHIStructureBuffer* StructBuffer)
{
	FD3D11StructureBuffer* D11StructureBuffer = (FD3D11StructureBuffer*)StructBuffer;
	D3D11_BUFFER_DESC BufferDesc;
	D11StructureBuffer->Resource->GetDesc(&BufferDesc);

	//ZYZ_TODO:ByteAccessBuffer Support later

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	SRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	SRVDesc.Buffer.NumElements = BufferDesc.ByteWidth/BufferDesc.StructureByteStride;
	SRVDesc.Buffer.FirstElement = 0;

	ID3D11ShaderResourceView* SRV;
	Direct3DDevice->CreateShaderResourceView(D11StructureBuffer->Resource , &SRVDesc , &SRV);

	return new FD3D11ShaderResourceView(SRV, D11StructureBuffer);
}

FRHIShaderResourceView* FD3D11DynamicRHI::RHICreateShaderResourceView(FRHIVertexBuffer* VertexBuffer, uint32 Stride, uint8 Format)
{
	FD3D11VertexBuffer* D11VertexBuffer = (FD3D11VertexBuffer*)VertexBuffer;
	D3D11_BUFFER_DESC BufferDesc;
	D11VertexBuffer->Resource->GetDesc(&BufferDesc);

	//ZYZ_TODO:ByteAccessBuffer Support later

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	SRVDesc.Format = FindShaderResourceDXGIFormat((DXGI_FORMAT)GPixelFormats[Format].PlatformFormat , false);
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	SRVDesc.Buffer.NumElements = BufferDesc.ByteWidth / Stride;
	SRVDesc.Buffer.FirstElement = 0;

	ID3D11ShaderResourceView* SRV;
	Direct3DDevice->CreateShaderResourceView(D11VertexBuffer->Resource, &SRVDesc, &SRV);

	return new FD3D11ShaderResourceView(SRV, D11VertexBuffer);
}

FRHIShaderResourceView* FD3D11DynamicRHI::RHICreateShaderResourceView(FRHIIndexBuffer* IndexBuffer)
{
	FD3D11IndexBuffer* D11IndexBuffer = (FD3D11IndexBuffer*)IndexBuffer;
	D3D11_BUFFER_DESC BufferDesc;
	D11IndexBuffer->Resource->GetDesc(&BufferDesc);

	//ZYZ_TODO:ByteAccessBuffer Support later

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	SRVDesc.Format = FindShaderResourceDXGIFormat((DXGI_FORMAT)GPixelFormats[(D11IndexBuffer->GetStride() == 2) ? PF_R16_UINT : PF_R32_UINT].PlatformFormat, false); ;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	SRVDesc.Buffer.NumElements = BufferDesc.ByteWidth / D11IndexBuffer->GetStride();
	SRVDesc.Buffer.FirstElement = 0;

	ID3D11ShaderResourceView* SRV;
	Direct3DDevice->CreateShaderResourceView(D11IndexBuffer->Resource, &SRVDesc, &SRV);

	return new FD3D11ShaderResourceView(SRV, D11IndexBuffer);
}

FRHIShaderResourceView* FD3D11DynamicRHI::RHICreateShaderResourceView(FRHITexture2D* Texture2D, uint8 MipLevel)
{
	FD3D11Texture2D* D11Texture2D = (FD3D11Texture2D*)Texture2D;
	ID3D11Texture2D* Texture = ((ID3D11Texture2D*)D11Texture2D->GetResource());
	D3D11_TEXTURE2D_DESC Texdesc;
	Texture->GetDesc(&Texdesc);
	ID3D11ShaderResourceView* SRV;
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;

	bool bSRGB = (D11Texture2D->GetFlags() & TexCreate_SRGB) != 0;
	SRVDesc.Format = FindShaderResourceDXGIFormat(Texdesc.Format , bSRGB);
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = 1;
	SRVDesc.Texture2D.MostDetailedMip = MipLevel;

	Direct3DDevice->CreateShaderResourceView(D11Texture2D->GetResource(), &SRVDesc, &SRV);
	return new FD3D11ShaderResourceView(SRV, D11Texture2D);
}

FRHIShaderResourceView* FD3D11DynamicRHI::RHICreateShaderResourceView(FRHITexture2D* Texture2D, uint8 MipLevel, uint8 NumMipLevels, uint8 Format)
{
	FD3D11Texture2D* D11Texture2D = (FD3D11Texture2D*)Texture2D;
	ID3D11Texture2D* Texture = ((ID3D11Texture2D*)D11Texture2D->GetResource());

	ID3D11ShaderResourceView* SRV;
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;

	bool bSRGB = (D11Texture2D->GetFlags() & TexCreate_SRGB) != 0;
	SRVDesc.Format = FindShaderResourceDXGIFormat((DXGI_FORMAT)GPixelFormats[Format].PlatformFormat, bSRGB);
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = NumMipLevels;
	SRVDesc.Texture2D.MostDetailedMip = MipLevel;

	Direct3DDevice->CreateShaderResourceView(D11Texture2D->GetResource(), &SRVDesc, &SRV);
	return new FD3D11ShaderResourceView(SRV, D11Texture2D);
}

FRHIShaderResourceView* FD3D11DynamicRHI::RHICreateShaderResourceView(FRHITexture3D* Texture3D, uint8 MipLevel)
{
	FD3D11Texture3D* D11Texture3D = (FD3D11Texture3D*)Texture3D;
	ID3D11Texture3D* Texture = ((ID3D11Texture3D*)D11Texture3D->GetResource());

	D3D11_TEXTURE3D_DESC desc;
	Texture->GetDesc(&desc);
	ID3D11ShaderResourceView* SRV;
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;

	bool bSRGB = (D11Texture3D->GetFlags() & TexCreate_SRGB) != 0;
	SRVDesc.Format = FindShaderResourceDXGIFormat((DXGI_FORMAT)GPixelFormats[desc.Format].PlatformFormat, bSRGB);
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
	SRVDesc.Texture3D.MipLevels = 0;
	SRVDesc.Texture3D.MostDetailedMip = MipLevel;

	Direct3DDevice->CreateShaderResourceView(D11Texture3D->GetResource(), &SRVDesc, &SRV);
	return new FD3D11ShaderResourceView(SRV, D11Texture3D);
}

FRHIShaderResourceView* FD3D11DynamicRHI::RHICreateShaderResourceView(FRHITexture2DArray* Texture2DArray, uint8 MipLevel)
{
	FD3D11Texture2DArray* D11Texture2DArray = (FD3D11Texture2DArray*)Texture2DArray;
	ID3D11Texture2D* Texture = D11Texture2DArray->GetResource();

	D3D11_TEXTURE2D_DESC desc;
	Texture->GetDesc(&desc);
	ID3D11ShaderResourceView* SRV;
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;

	bool bSRGB = (Texture2DArray->GetFlags() & TexCreate_SRGB) != 0;
	SRVDesc.Format = FindShaderResourceDXGIFormat((DXGI_FORMAT)GPixelFormats[desc.Format].PlatformFormat, bSRGB);
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	SRVDesc.Texture2DArray.ArraySize = desc.ArraySize;
	SRVDesc.Texture2DArray.FirstArraySlice = 0;
	SRVDesc.Texture2DArray.MipLevels = desc.MipLevels;
	SRVDesc.Texture2DArray.MostDetailedMip = MipLevel;

	Direct3DDevice->CreateShaderResourceView(D11Texture2DArray->GetResource(), &SRVDesc, &SRV);
	return new FD3D11ShaderResourceView(SRV, D11Texture2DArray);
}

FRHIShaderResourceView* FD3D11DynamicRHI::RHICreateShaderResourceView(FRHITextureCube* TextureCube, uint8 MipLevel)
{
	FD3D11TextureCube* D11Texture2DArray = (FD3D11TextureCube*)TextureCube;
	ID3D11Texture2D* Texture = D11Texture2DArray->GetResource();

	D3D11_TEXTURE2D_DESC desc;
	Texture->GetDesc(&desc);
	ID3D11ShaderResourceView* SRV;
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;

	bool bSRGB = (TextureCube->GetFlags() & TexCreate_SRGB) != 0;
	SRVDesc.Format = FindShaderResourceDXGIFormat((DXGI_FORMAT)GPixelFormats[desc.Format].PlatformFormat, bSRGB);
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	SRVDesc.TextureCube.MipLevels = desc.MipLevels;
	SRVDesc.TextureCube.MostDetailedMip = MipLevel;

	Direct3DDevice->CreateShaderResourceView(D11Texture2DArray->GetResource(), &SRVDesc, &SRV);
	return new FD3D11ShaderResourceView(SRV, D11Texture2DArray);
}

template <EShaderFrequency ShaderFrequency>
void FD3D11DynamicRHI::ClearAllShaderResourcesForFrequency()
{
	int maxSRVNum = MaxBoundShaderResourcesIndex[ShaderFrequency];
	for (int index = maxSRVNum; index >= 0;index--)
	{
		if (CurrentResourcesBoundAsSRVs[ShaderFrequency][index])
		{
			InternalSetShaderResourceView<ShaderFrequency>(NULL , NULL , index , "");
		}
	}
}

template <EShaderFrequency ShaderFrequency>
void FD3D11DynamicRHI::InternalSetShaderResourceView(FD3D11BaseShaderResource* Resource, ID3D11ShaderResourceView* SRV, int32 ResourceIndex, std::string SRVName, FD3D11StateCache::ESRV_Type SrvType)
{
	FD3D11BaseShaderResource* D11SRV = CurrentResourcesBoundAsSRVs[ShaderFrequency][ResourceIndex];
	int& MaxResource = MaxBoundShaderResourcesIndex[ShaderFrequency];

	if (Resource)
	{
		MaxResource = FMath::Max(ResourceIndex , MaxResource);
		D11SRV = Resource;
	}
	else if (D11SRV != nullptr)
	{
		//Clear
		D11SRV = nullptr;

		if (ResourceIndex == MaxResource)
		{
			do
			{
				MaxResource--;
			} while (MaxResource >= 0 && CurrentResourcesBoundAsSRVs[ShaderFrequency][MaxResource] == nullptr);
		}

	}

	StateCache.SetShaderResourceView<ShaderFrequency>(SRV, ResourceIndex , SrvType);
}

template <EShaderFrequency ShaderFrequency>
void FD3D11DynamicRHI::ClearShaderResourceViews(FD3D11BaseShaderResource* Resource)
{
	for (int index = MaxBoundShaderResourcesIndex[ShaderFrequency]; index >= 0; --index)
	{
		if (CurrentResourcesBoundAsSRVs[ShaderFrequency][index] == Resource)
		{
			InternalSetShaderResourceView<ShaderFrequency>(nullptr, nullptr, index, "");
		}
	}
}

void FD3D11DynamicRHI::CommitRenderTargetsAndUAVs()
{
	//Copy RTV Pointer
	ID3D11RenderTargetView* RTVs[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
	for (int i = 0; i < NumSimultaneousRenderTargets;i++)
	{
		RTVs[i] = CurrentRenderTargets[i];
	}

	//Copy UAV Pointer
	uint32 UAVInits[D3D11_PS_CS_UAV_REGISTER_COUNT];
	ID3D11UnorderedAccessView* UAVs[D3D11_PS_CS_UAV_REGISTER_COUNT];
	for (int i = 0; i < NumUAVs; i++)
	{
		UAVs[i] = CurrentUAVs[i];
		UAVInits[i] = -1;
	}

	if (NumUAVs > 0)
	{
		Direct3DDeviceIMContext->OMSetRenderTargetsAndUnorderedAccessViews(NumSimultaneousRenderTargets, RTVs, CurrentDepthStencilTarget, NumSimultaneousRenderTargets, NumUAVs
			, UAVs,UAVInits);
	}
	else
	{
		Direct3DDeviceIMContext->OMSetRenderTargets(NumSimultaneousRenderTargets, RTVs, CurrentDepthStencilTarget);
	}

}

void FD3D11DynamicRHI::CommitGraphicResourceTables()
{
	FD3D11BoundShaderState* CurrentBoundShaderState = (FD3D11BoundShaderState*)BoundShaderStateHistory.GetLast();

	if (CurrentBoundShaderState->VertexShader)
	{
		SetResourcesFromTables(CurrentBoundShaderState->GetVertexShader());
	}
	if (CurrentBoundShaderState->PixelShader)
	{
		SetResourcesFromTables(CurrentBoundShaderState->GetPixelShader());
	}
	if (CurrentBoundShaderState->DomainShader)
	{
		SetResourcesFromTables(CurrentBoundShaderState->GetDomainShader());
	}
	if (CurrentBoundShaderState->HullShader)
	{
		SetResourcesFromTables(CurrentBoundShaderState->GetHullShader());
	}
	if (CurrentBoundShaderState->GeometryShader)
	{
		SetResourcesFromTables(CurrentBoundShaderState->GetGeometryShader());
	}
}

void FD3D11DynamicRHI::CommitComputeShaderResourceTables(FD3D11ComputeShader* ComputeShader)
{

}

void FD3D11DynamicRHI::CommitNonComputeShaderConstants()
{

}

void FD3D11DynamicRHI::CommitComputeShaderConstants()
{

}

void FD3D11DynamicRHI::ClearAllShaderResources()
{
	ClearAllShaderResourcesForFrequency<SF_Vertex>();
	ClearAllShaderResourcesForFrequency<SF_Hull>();
	ClearAllShaderResourcesForFrequency<SF_Domain>();
	ClearAllShaderResourcesForFrequency<SF_Pixel>();
	ClearAllShaderResourcesForFrequency<SF_Geometry>();
	ClearAllShaderResourcesForFrequency<SF_Compute>();
}

void FD3D11DynamicRHI::ConditionalClearShaderResource(FD3D11BaseShaderResource* Resource)
{
	ClearShaderResourceViews<SF_Vertex>(Resource);
	ClearShaderResourceViews<SF_Hull>(Resource);
	ClearShaderResourceViews<SF_Domain>(Resource);
	ClearShaderResourceViews<SF_Pixel>(Resource);
	ClearShaderResourceViews<SF_Geometry>(Resource);
	ClearShaderResourceViews<SF_Compute>(Resource);
}