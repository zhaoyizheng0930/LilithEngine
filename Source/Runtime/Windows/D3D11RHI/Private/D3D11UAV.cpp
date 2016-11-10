#include "D3D11RHIPCH.h"
#include "WindowsD3D11DynamicRHI.h"
#include "RenderUtils.h"


FRHIUnorderedAccessView* FD3D11DynamicRHI::RHICreateUnorderedAccessView(FRHITexture* Texture, uint32 MipLevel)
{
	FD3D11UnorderedAccessView* D11UnordererAccessView = NULL;
	FD3D11TextureBase* D11Texture = (FD3D11TextureBase*)Texture;
	ID3D11UnorderedAccessView* UnorderedAccessView = nullptr;
	ID3D11Resource* Resource = NULL;
	D3D11_UNORDERED_ACCESS_VIEW_DESC UnorderedAccessViewDesc;
	UnorderedAccessViewDesc.Format = FindShaderResourceDXGIFormat((DXGI_FORMAT)GPixelFormats[Texture->GetPixelFormat()].PlatformFormat , false);

	if (Texture->GetTexture2DArray() != NULL)
	{
		Resource = ((FD3D11Texture2DArray*)Texture)->GetResource();
		UnorderedAccessViewDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
		UnorderedAccessViewDesc.Texture2DArray.MipSlice = MipLevel;
		UnorderedAccessViewDesc.Texture2DArray.FirstArraySlice = 0;
		UnorderedAccessViewDesc.Texture2DArray.ArraySize = ((FD3D11Texture2DArray*)Texture)->GetSizeZ();
	}
	else if (Texture->GetTexture3D() != NULL)
	{
		Resource = ((FD3D11Texture3D*)Texture)->GetResource();
		UnorderedAccessViewDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE3D;
		UnorderedAccessViewDesc.Texture3D.FirstWSlice = 0;
		UnorderedAccessViewDesc.Texture3D.MipSlice = MipLevel;
		UnorderedAccessViewDesc.Texture3D.WSize = ((FD3D11Texture3D*)Texture)->GetSizeZ() >> MipLevel;
	}
	else if (Texture->GetTextureCube() != NULL)
	{
		Resource = ((FD3D11TextureCube*)Texture)->GetResource();
		UnorderedAccessViewDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
		UnorderedAccessViewDesc.Texture2DArray.ArraySize = 6;
		UnorderedAccessViewDesc.Texture2DArray.FirstArraySlice = 0;
		UnorderedAccessViewDesc.Texture2DArray.MipSlice = MipLevel;
	}
	else
	{
		Resource = ((FD3D11Texture2D*)Texture)->GetResource();
		UnorderedAccessViewDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		UnorderedAccessViewDesc.Texture2D.MipSlice = MipLevel;
	}

	Direct3DDevice->CreateUnorderedAccessView(Resource , &UnorderedAccessViewDesc , &UnorderedAccessView);
	D11UnordererAccessView = new FD3D11UnorderedAccessView(UnorderedAccessView, D11Texture);
	return D11UnordererAccessView;
}

FRHIUnorderedAccessView* FD3D11DynamicRHI::RHICreateUnorderedAccessView(FRHIStructureBuffer* StructBuffer, bool bUseUAVCounter, bool bAppendBuffer)
{
	FD3D11UnorderedAccessView* D11UnorderedAccessView = NULL;
	FD3D11StructureBuffer* D11StructureBuffer = (FD3D11StructureBuffer*)StructBuffer;
	D3D11_BUFFER_DESC Bufferdesc;
	D11StructureBuffer->Resource->GetDesc(&Bufferdesc);
	const bool bByteAccessBuffer = (Bufferdesc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS) != 0;

	D3D11_UNORDERED_ACCESS_VIEW_DESC UAVdesc;
	UAVdesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	UAVdesc.Format = DXGI_FORMAT_UNKNOWN;
	UAVdesc.Buffer.FirstElement = 0;
	const uint32 EffectiveStride = Bufferdesc.StructureByteStride == 0 ? 4 : Bufferdesc.StructureByteStride;
	UAVdesc.Buffer.NumElements = Bufferdesc.ByteWidth / EffectiveStride;
	UAVdesc.Buffer.Flags = 0;

	if (bUseUAVCounter)
	{
		UAVdesc.Buffer.Flags |= D3D11_BUFFER_UAV_FLAG_COUNTER;
	}
	if (bAppendBuffer)
	{
		UAVdesc.Buffer.Flags |= D3D11_BUFFER_UAV_FLAG_APPEND;
	}
	if (bByteAccessBuffer)
	{
		UAVdesc.Buffer.Flags |= D3D11_BUFFER_UAV_FLAG_RAW;
	}

	ID3D11UnorderedAccessView* UnorderedAccessView = nullptr;
	Direct3DDevice->CreateUnorderedAccessView(D11StructureBuffer->Resource, &UAVdesc, &UnorderedAccessView);

	D11UnorderedAccessView = new FD3D11UnorderedAccessView(UnorderedAccessView , D11StructureBuffer);
	return D11UnorderedAccessView;
}

FRHIUnorderedAccessView* FD3D11DynamicRHI::RHICreateUnorderedAccessView(FRHIVertexBuffer* VertexBuffer, uint8 Format)
{
	FD3D11UnorderedAccessView* D11UAV = NULL;
	FD3D11VertexBuffer* D11VertexBuffer = (FD3D11VertexBuffer*)VertexBuffer;
	ID3D11Buffer* Vertexbuffer = D11VertexBuffer->Resource;
	D3D11_BUFFER_DESC bufferdesc;
	Vertexbuffer->GetDesc(&bufferdesc);

	//ZYZ_TODO:Support bByteAccessBuffer later!

	D3D11_UNORDERED_ACCESS_VIEW_DESC UAVdesc;
	UAVdesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	UAVdesc.Format = FindUnorderedAccessDXGIFormat((DXGI_FORMAT)GPixelFormats[Format].PlatformFormat);
	UAVdesc.Buffer.FirstElement = 0;
	UAVdesc.Buffer.NumElements = bufferdesc.ByteWidth / GPixelFormats[Format].BlockBytes;
	UAVdesc.Buffer.Flags = 0;

	ID3D11UnorderedAccessView* UnorderedAccessView = nullptr;
	Direct3DDevice->CreateUnorderedAccessView(Vertexbuffer, &UAVdesc, &UnorderedAccessView);

	D11UAV = new FD3D11UnorderedAccessView(UnorderedAccessView, D11VertexBuffer);
	return D11UAV;
}

void FD3D11DynamicRHI::RHIClearUAV(FRHIUnorderedAccessView* UAV, uint32* Values)
{
	FD3D11UnorderedAccessView* D11UAV = (FD3D11UnorderedAccessView*)UAV;

	Direct3DDeviceIMContext->ClearUnorderedAccessViewUint(D11UAV->View, Values);
}