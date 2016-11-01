#include "D3D11RHIPCH.h"
#include "D3D11Resources.h"
#include "WindowsD3D11DynamicRHI.h"

template<typename BaseResourceType>
TD3D11Texture2D<BaseResourceType>* FD3D11DynamicRHI::CreateD3D11Texture2D(uint32 SizeX, uint32 SizeY, uint32 SizeZ, bool bTextureArray, bool bCubeTexture, uint8 Format,
	uint32 NumMips, uint32 NumSamples, uint32 Flags, FRHIResourceCreateInfo& CreateInfo)
{
	//Determine the MSAA
	D3D11_DSV_DIMENSION DepthStencilViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	D3D11_RTV_DIMENSION RenderTargetViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	D3D11_SRV_DIMENSION ShaderResourceViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	D3D11_USAGE TextureUsage = D3D11_USAGE_DEFAULT;
	uint32 BindFlag = D3D11_BIND_SHADER_RESOURCE;
	bool bCreateShaderResource = true;
	uint32 CPUAccessFlags = 0;

	uint32 ActualMSAACount = NumSamples;
	uint32 ActualMSAAQuality = GetMaxMSAAQuality(ActualMSAACount);

	if (ActualMSAACount == 0xffffffff || (Flags & TexCreate_Shared) != 0)
	{
		//No MSAA
		ActualMSAACount = 1;
		ActualMSAAQuality = 0;
	}

	if (ActualMSAACount > 1)
	{
		DepthStencilViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		RenderTargetViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
		ShaderResourceViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
	}

	if (Flags & TexCreate_CPUReadback)
	{
		CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		TextureUsage = D3D11_USAGE_STAGING;
		BindFlags = 0;
		bCreateShaderResource = false;
	}

	D3D11_TEXTURE2D_DESC TexDesc;
	TexDesc.Width = SizeX;
	TexDesc.Height = SizeY;
	TexDesc.MipLevels = NumMips;
	TexDesc.ArraySize = SizeZ;
	TexDesc.Format = GPixelFormats[Format].PlatformFormat;
	TexDesc.SampleDesc.Count = ActualMSAACount;
	TexDesc.SampleDesc.Quality = ActualMSAAQuality;
	TexDesc.Usage = TextureUsage;
	TexDesc.BindFlags = BindFlag;
	TexDesc.CPUAccessFlags = CPUAccessFlags;
	TexDesc.MiscFlags = bCubeTexture ? D3D11_RESOURCE_MISC_TEXTURECUBE : 0;

	if (Flags & TexCreate_Shared)
	{
		TexDesc.MiscFlags |= D3D11_RESOURCE_MISC_SHARED;
	}

	if (Flags & TexCreate_GenerateMipCapable)
	{
		// Set the flag that allows us to call GenerateMips on this texture later
		TexDesc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
	}

	bool bCreateRTV = false;
	bool bCreateDSV = false;
	bool bCreatedRTVPerSlice = false;

	if (Flags & TexCreate_RenderTargetable)
	{
		TexDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
		bCreateRTV = true;
	}
	else if (Flags & TexCreate_DepthStencilTargetable)
	{
		TexDesc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
		bCreateDSV = true;
	}
	else if (Flags & TexCreate_ResolveTargetable)
	{
		if (Format == PF_DepthStencil || Format == PF_ShadowDepth || Format == PF_D24)
		{
			TexDesc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
			bCreateDSV = true;
		}
		else
		{
			TexDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
			bCreateRTV = true;
		}
	}

	if (Flags & TexCreate_UAV)
	{
		TexDesc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
	}

	ID3D11Texture2D* TextureResource = NULL;
	ID3D11ShaderResourceView* ShaderResourceView = NULL;
	std::vector<ID3D11RenderTargetView*> RenderTargetViews = NULL;
	ID3D11DepthStencilView* DepthStencilViews[4];

	D3D11_SUBRESOURCE_DATA* SubResource;
	//Init Later
	Direct3DDevice->CreateTexture2D(TexDesc, NULL, &TextureResource);

	//CreateRTV
	if (bCreateRTV)
	{
		for (uint32 MipIndex = 0; MipIndex < NumMips; MipIndex++)
		{
			if ((Flags & TexCreate_TargetArraySlicesIndependently) && (bTextureArray || bCubeTexture))
			{
				for (uint32 SliceIndex = 0; SliceIndex < TexDesc.ArraySize;SliceIndex++)
				{
					ID3D11RenderTargetView* RenderTarget = NULL;
					D3D11_RENDER_TARGET_VIEW_DESC RenderTargetDesc;
					RenderTargetDesc.Format = FindShaderResourceDXGIFormat(GPixelFormats[Format].PlatformFormat);
					RenderTargetDesc.ViewDimension = RenderTargetViewDimension;
					RenderTargetDesc.Texture2DArray.ArraySize = 1;
					RenderTargetDesc.Texture2DArray.MipSlice = MipIndex;
					RenderTargetDesc.Texture2DArray.FirstArraySlice = SliceIndex;
					Direct3DDevice->CreateRenderTargetView(TextureResource, &RenderTargetDesc, &RenderTarget);
					RenderTargetViews.push_back(RenderTarget);
				}
			}
			else
			{
				D3D11_RENDER_TARGET_VIEW_DESC RenderTargetDesc;
				RenderTargetDesc.Format = FindShaderResourceDXGIFormat(GPixelFormats[Format].PlatformFormat);
				if (bTextureArray || bCubeTexture)
				{
					RenderTargetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
					RenderTargetDesc.Texture2DArray.FirstArraySlice = 0;
					RenderTargetDesc.Texture2DArray.ArraySize = TexDesc.ArraySize;
					RenderTargetDesc.Texture2DArray.MipSlice = MipIndex;
				}
				else
				{
					RenderTargetDesc.ViewDimension = RenderTargetViewDimension;
					RenderTargetDesc.Texture2D.MipSlice = MipIndex;
				}

				ID3D11RenderTargetView* RenderTarget = NULL;
				Direct3DDevice->CreateRenderTargetView(TextureResource, &RenderTargetDesc, &RenderTarget);
				RenderTargetViews.push_back(RenderTarget);
			}
		}
	}

	//CreateDSV
	if (bCreateDSV)
	{

	}

	//CreateSRV
	if (bCreateShaderResource)
	{
	}
}

uint32 FD3D11DynamicRHI::GetMaxMSAAQuality(int InActualMSAACount)
{
	if (InActualMSAACount < 8)
	{
		return 0;
	}
	else
	{
		return 0xffffffff;
	}
}

FRHITexture2D* FD3D11DynamicRHI::RHICreateTexture2D(uint32 SizeX, uint32 SizeY, uint8 Format, uint32 NumMips, uint32 NumSamples, uint32 Flags, FRHIResourceCreateInfo& CreateInfo)
{
	FD3D11Texture2D* Texture2D = CreateD3D11Texture2D<FD3D11BaseTexture2D>(SizeX , SizeY , 1 ,false ,false,Format , NumMips , NumSamples , Flags , CreateInfo);
}

template <class RHIResourceType>
void* TD3D11Texture2D<RHIResourceType>::Lock(uint32 MipIndex, uint32 ArrayIndex, EResourceLockMode LockMode, uint32& DestStride)
{

}

template <class RHIResourceType>
void TD3D11Texture2D<RHIResourceType>::Unlock(uint32 MipIndex, uint32 ArrayIndex)
{

}