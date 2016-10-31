#include "D3D11RHIPCH.h"
#include "WindowsD3D11DynamicRHI.h"


FRHIUnorderedAccessView* FD3D11DynamicRHI::RHICreateUnorderedAccessView(FRHITexture* Texture, uint32 MipLevel)
{
	//FD3D11UnorderedAccessView* D11UnordererAccessView = NULL;
	//FD3D11TextureBase* D11Texture = (FD3D11TextureBase*)Texture;
	//ID3D11UnorderedAccessView* UnorderedAccessView = nullptr;
	//if (Texture->GetTexture2D() != NULL)
	//{
	//	//FRHITexture2D
	//}
	//else if (Texture->GetTexture2DArray() != NULL)
	//{
	//}
	//else if (Texture->GetTexture3D() != NULL)
	//{
	//}
	//else if (Texture->GetTextureCube() != NULL)
	//{
	//}
	//else if (Texture->GetTextureReference() != NULL)
	//{
	//}
	//Direct3DDevice->CreateUnorderedAccessView(D11Texture->)
	//D11UnordererAccessView = new FD3D11UnorderedAccessView( , Texture);
	//return D11UnordererAccessView;
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
}

FRHIUnorderedAccessView* FD3D11DynamicRHI::RHICreateUnorderedAccessView(FRHIVertexBuffer* VertexBuffer, uint8 Format)
{

}