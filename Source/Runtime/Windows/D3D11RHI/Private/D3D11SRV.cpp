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