#include "D3D11RHIPCH.h"
#include "D3D11Resources.h"
#include "WindowsD3D11DynamicRHI.h"
#include "RenderUtils.h"

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
		BindFlag = 0;
		bCreateShaderResource = false;
	}

	D3D11_TEXTURE2D_DESC TexDesc;
	TexDesc.Width = SizeX;
	TexDesc.Height = SizeY;
	TexDesc.MipLevels = NumMips;
	TexDesc.ArraySize = SizeZ;
	TexDesc.Format = (DXGI_FORMAT)GPixelFormats[Format].PlatformFormat;
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
	std::vector<ID3D11RenderTargetView*> RenderTargetViews;
	std::vector<ID3D11DepthStencilView*> DepthStencilViews;

	//D3D11_SUBRESOURCE_DATA* SubResource;
	//Init Later
	Direct3DDevice->CreateTexture2D(&TexDesc, NULL, &TextureResource);

	//CreateRTV
	if (bCreateRTV)
	{
		for (uint32 MipIndex = 0; MipIndex < NumMips; MipIndex++)
		{
			if ((Flags & TexCreate_TargetArraySlicesIndependently) && (bTextureArray || bCubeTexture))
			{
				bCreatedRTVPerSlice = true;
				for (uint32 SliceIndex = 0; SliceIndex < TexDesc.ArraySize;SliceIndex++)
				{
					ID3D11RenderTargetView* RenderTarget = NULL;
					D3D11_RENDER_TARGET_VIEW_DESC RenderTargetDesc;
					RenderTargetDesc.Format = FindShaderResourceDXGIFormat((DXGI_FORMAT)GPixelFormats[Format].PlatformFormat , (Flags & TexCreate_SRGB) != 0);
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
				RenderTargetDesc.Format = FindShaderResourceDXGIFormat((DXGI_FORMAT)GPixelFormats[Format].PlatformFormat , (Flags & TexCreate_SRGB) != 0);
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
		D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc;
		DSVDesc.Format = FindDepthStencilDXGIFormat((DXGI_FORMAT)GPixelFormats[Format].PlatformFormat);
		if (bTextureArray || bCubeTexture)
		{
			DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
			DSVDesc.Texture2DArray.ArraySize = TexDesc.ArraySize;
			DSVDesc.Texture2DArray.FirstArraySlice = 0;
			DSVDesc.Texture2DArray.MipSlice = 0;
		}
		else
		{
			DSVDesc.ViewDimension = DepthStencilViewDimension;
			DSVDesc.Texture2D.MipSlice = 0;
		}

		for (int AccessType = 0; AccessType < 4; ++AccessType)
		{
			if (Direct3DDevice->GetFeatureLevel() == D3D_FEATURE_LEVEL_11_0)
			{
				DSVDesc.Flags = D3D11_DSV_READ_ONLY_DEPTH;
				if (DSVDesc.Format == DXGI_FORMAT_D24_UNORM_S8_UINT)
				{
					DSVDesc.Flags = D3D11_DSV_READ_ONLY_STENCIL;
				}
			}
			
			ID3D11DepthStencilView* DepthStencilView = NULL;
			Direct3DDevice->CreateDepthStencilView(TextureResource, &DSVDesc, &DepthStencilView);
			DepthStencilViews.push_back(DepthStencilView);
		}
	}

	//CreateSRV
	if (bCreateShaderResource)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		SRVDesc.Format = (DXGI_FORMAT)GPixelFormats[Format].PlatformFormat;
		if (bTextureArray && bCubeTexture)
		{
			SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
			SRVDesc.TextureCubeArray.First2DArrayFace = 0;
			SRVDesc.TextureCubeArray.MostDetailedMip = 0;
			SRVDesc.TextureCubeArray.MipLevels = NumMips;
			SRVDesc.TextureCubeArray.NumCubes = SizeZ / 6;
		}
		else if(bTextureArray)
		{
			SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
			SRVDesc.Texture2DArray.ArraySize = TexDesc.ArraySize;
			SRVDesc.Texture2DArray.FirstArraySlice = 0;
			SRVDesc.Texture2DArray.MipLevels = NumMips;
			SRVDesc.Texture2DArray.MostDetailedMip = 0;
		}
		else if(bCubeTexture)
		{
			SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			SRVDesc.TextureCube.MostDetailedMip = 0;
			SRVDesc.TextureCube.MipLevels = NumMips;
		}
		else
		{
			SRVDesc.ViewDimension = ShaderResourceViewDimension;
			SRVDesc.Texture2D.MostDetailedMip = 0;
			SRVDesc.Texture2D.MipLevels = NumMips;
		}

		Direct3DDevice->CreateShaderResourceView(TextureResource , &SRVDesc , &ShaderResourceView);
	}

	//Create RHITexture2D
	TD3D11Texture2D<BaseResourceType>* RHITexture2D = new TD3D11Texture2D<BaseResourceType>(this,
		TextureResource,
		ShaderResourceView,
		RenderTargetViews.size(),
		bCreatedRTVPerSlice,
		RenderTargetViews,
		DepthStencilViews,
		SizeX,
		SizeY,
		SizeZ,
		NumMips,
		NumSamples,
		(EPixelFormat)Format,
		Flags);

	//Wait for Memory Calculate.
	//D3D11TextureAllocated();

	return RHITexture2D;
}

FD3D11Texture3D* FD3D11DynamicRHI::CreateD3D11Texture3D(uint32 SizeX, uint32 SizeY, uint32 SizeZ, uint8 Format, uint32 NumMips, uint32 Flags)
{
	ID3D11Texture3D* NewTexture3D = NULL;
	ID3D11ShaderResourceView* ShaderResourceView = NULL;
	std::vector<ID3D11RenderTargetView*> RenderTargetViews;

	bool bCreateRTV = false;
	//CreateTexture
	D3D11_TEXTURE3D_DESC TextureDesc;
	TextureDesc.Width = SizeX;
	TextureDesc.Height = SizeY;
	TextureDesc.Depth = SizeZ;
	TextureDesc.MipLevels = NumMips;
	TextureDesc.Format = (DXGI_FORMAT)GPixelFormats[Format].PlatformFormat;
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (Flags & TexCreate_GenerateMipCapable)
	{
		TextureDesc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
	}

	if (Flags & TexCreate_UAV)
	{
		TextureDesc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
	}

	if (Flags & TexCreate_RenderTargetable)
	{
		bCreateRTV = true;
		TextureDesc.MiscFlags |= D3D11_BIND_RENDER_TARGET;
	}

	//Init Data support later
	Direct3DDevice->CreateTexture3D(&TextureDesc,NULL ,&NewTexture3D);
	//CreateSRV
	D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc;
	ShaderResourceViewDesc.Format = FindShaderResourceDXGIFormat((DXGI_FORMAT)GPixelFormats[Format].PlatformFormat, (Flags & TexCreate_SRGB)!= 0);
	ShaderResourceViewDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE3D;
	ShaderResourceViewDesc.Texture3D.MipLevels = NumMips;
	ShaderResourceViewDesc.Texture3D.MostDetailedMip = 0;

	Direct3DDevice->CreateShaderResourceView(NewTexture3D , &ShaderResourceViewDesc , &ShaderResourceView);
	//CreateRTV
	if (bCreateRTV)
	{
		ID3D11RenderTargetView* RenderTargetView = NULL;
		D3D11_RENDER_TARGET_VIEW_DESC RenderTargetViewDesc;
		RenderTargetViewDesc.Format = FindShaderResourceDXGIFormat((DXGI_FORMAT)GPixelFormats[Format].PlatformFormat, (Flags & TexCreate_SRGB) != 0);
		RenderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE3D;
		RenderTargetViewDesc.Texture3D.FirstWSlice = 0;
		RenderTargetViewDesc.Texture3D.WSize = SizeZ;
		RenderTargetViewDesc.Texture3D.MipSlice = 0;

		Direct3DDevice->CreateRenderTargetView(NewTexture3D, &RenderTargetViewDesc , &RenderTargetView);
		RenderTargetViews.push_back(RenderTargetView);
	}

	FD3D11Texture3D* D11Texture3D = new FD3D11Texture3D(this,
		NewTexture3D,
		ShaderResourceView,
		RenderTargetViews,
		SizeX,
		SizeY,
		SizeZ,
		NumMips,
		(EPixelFormat)Format,
		Flags);

	//Wait for Memory Calculate.
	//D3D11TextureAllocated();

	return D11Texture3D;
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
	return Texture2D;
}

FRHITexture2D* FD3D11DynamicRHI::RHIAsyncCreateTexture2D(uint32 SizeX, uint32 SizeY, uint8 Format, uint32 NumMips, uint32 Flags, void** InitialMipData, uint32 NumInitialMips)
{
	FD3D11Texture2D* NewTexture = NULL;
	ID3D11ShaderResourceView* ShaderResourceView = NULL;
	std::vector<ID3D11RenderTargetView*> RenderTargetViews;

	//CreateTexture-------------------------------------------------------------------------
	D3D11_TEXTURE2D_DESC Texture2dDesc;
	Texture2dDesc.Width = SizeX;
	Texture2dDesc.Height = SizeY;
	Texture2dDesc.MipLevels = NumMips;
	Texture2dDesc.ArraySize = 1;
	Texture2dDesc.Format = (DXGI_FORMAT)(GPixelFormats[Format].PlatformFormat);
	Texture2dDesc.SampleDesc.Count = 1;
	Texture2dDesc.SampleDesc.Quality = 0;
	Texture2dDesc.Usage = D3D11_USAGE_DEFAULT;
	Texture2dDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	Texture2dDesc.MiscFlags = 0;
	Texture2dDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA D3D11SubResources[14];
	//InitData
	for (uint32 i = 0;i<NumInitialMips;i++)
	{
		uint32 NumBlocksX = FMath::Max<uint32>(1, (SizeX >> i) / GPixelFormats[Format].BlockSizeX);
		uint32 NumBlocksY = FMath::Max<uint32>(1, (SizeY >> i) / GPixelFormats[Format].BlockSizeY);

		D3D11SubResources[i].pSysMem = InitialMipData[i];
		D3D11SubResources[i].SysMemPitch = NumBlocksX * GPixelFormats[Format].BlockBytes;
		D3D11SubResources[i].SysMemSlicePitch = NumBlocksY * NumBlocksX * GPixelFormats[Format].BlockBytes;
	}

	//Init Other Mip data
	for (uint32 i = NumInitialMips;i<14;i++)
	{
		uint32 NumBlocksX = FMath::Max<uint32>(1, (SizeX >> i) / GPixelFormats[Format].BlockSizeX);
		uint32 NumBlocksY = FMath::Max<uint32>(1, (SizeY >> i) / GPixelFormats[Format].BlockSizeY);

		uint32 MipSize = NumBlocksX * NumBlocksY * GPixelFormats[Format].BlockBytes;
		/*void* TempBuffer = ZeroMemory;
		if (MipSize > ZeroMemorySize)
		{
			TempBuffer = FMemory::Malloc(MipSize);
			FMemory::Memzero(TempBuffer , MipSize);
		}*/
		D3D11SubResources[i].pSysMem = NULL;
		D3D11SubResources[i].SysMemPitch = NumBlocksX * GPixelFormats[Format].BlockBytes;
		D3D11SubResources[i].SysMemSlicePitch = MipSize;
	}

	ID3D11Texture2D* Texture;
	Direct3DDevice->CreateTexture2D(&Texture2dDesc , D3D11SubResources , &Texture);
	/*if (TempBufferSize != ZeroBufferSize)
	{
	FMemory::Free(TempBuffer);
	}*/

	//Create Shader Resource View-------------------------------------------------------------------------
	D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc;
	ShaderResourceViewDesc.Format = FindShaderResourceDXGIFormat((DXGI_FORMAT)GPixelFormats[Format].PlatformFormat, ((Flags & TexCreate_SRGB) != 0));
	ShaderResourceViewDesc.Texture2D.MipLevels = NumMips;
	ShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	ShaderResourceViewDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;

	Direct3DDevice->CreateShaderResourceView(Texture, &ShaderResourceViewDesc, &ShaderResourceView);

	NewTexture = new FD3D11Texture2D(this,
		Texture,
		ShaderResourceView,
		1,
		false,
		RenderTargetViews,
		std::vector<ID3D11DepthStencilView*>(),
		SizeX,
		SizeY,
		0,
		NumMips,
		1,
		(EPixelFormat)Format,
		Flags);

	//Wait for Memory Calculate.
	//D3D11TextureAllocated();

	return NewTexture;
}

void FD3D11DynamicRHI::RHICopySharedMips(FRHITexture2D* DestTexture2D, FRHITexture2D* SrcTexture2D)
{
	FD3D11Texture2D* D11DestTexture2D = (FD3D11Texture2D*)DestTexture2D;
	FD3D11Texture2D* D11SrcTexture2D = (FD3D11Texture2D*)SrcTexture2D;

	int NumSharedMips = FMath::Min(D11DestTexture2D->GetNumMips(), D11DestTexture2D->GetNumMips());

	for (int MipIndex = 0; MipIndex < NumSharedMips; MipIndex++)
	{
		Direct3DDeviceIMContext->CopySubresourceRegion(D11DestTexture2D->GetResource() ,
			D3D11CalcSubresource(MipIndex + (D11DestTexture2D->GetNumMips() - NumSharedMips),0 ,D11DestTexture2D->GetNumMips()),
			0,
			0,
			0,
			D11SrcTexture2D->GetResource(),
			D3D11CalcSubresource(MipIndex + (D11SrcTexture2D->GetNumMips() - NumSharedMips), 0, D11SrcTexture2D->GetNumMips()) ,
			NULL);
	}
}

FRHITexture2DArray* FD3D11DynamicRHI::RHICreateTexture2DArray(uint32 SizeX, uint32 SizeY, uint32 SizeZ, uint8 Format, uint32 NumMips, uint32 Flags, FRHIResourceCreateInfo& CreateInfo)
{
	return CreateD3D11Texture2D<FD3D11BaseTexture2DArray>(SizeX ,SizeY , SizeZ , true , false , Format , NumMips ,1 , Flags , CreateInfo );
}

FRHITexture3D* FD3D11DynamicRHI::RHICreateTexture3D(uint32 SizeX, uint32 SizeY, uint32 SizeZ, uint8 Format, uint32 NumMips, uint32 NumSamples, uint32 Flags, FRHIResourceCreateInfo& CreateInfo)
{
	return CreateD3D11Texture3D(SizeX, SizeY, SizeZ, Format, NumMips, Flags);
}

FRHITextureCube* FD3D11DynamicRHI::RHICreateTextureCube(uint32 Size, uint8 Format, uint32 NumMips, uint32 Flags, FRHIResourceCreateInfo& CreateInfo)
{
	return  CreateD3D11Texture2D<FD3D11BaseTextureCube>(Size, Size, 6, false, true, Format, NumMips, 1, Flags, CreateInfo);
}

FRHITextureCube* FD3D11DynamicRHI::RHICreateTextureCubeArray(uint32 Size, uint32 ArraySize, uint8 Format, uint32 NumMips, uint32 Flags, FRHIResourceCreateInfo& CreateInfo)
{
	return CreateD3D11Texture2D<FD3D11BaseTextureCube>(Size , Size , 6 * ArraySize, true , true ,Format , NumMips , 1 ,Flags , CreateInfo);
}

uint64 FD3D11DynamicRHI::RHICalcTexture2DPlatformSize(uint32 SizeX, uint32 SizeY, uint8 Format, uint32 NumMips, uint32 Flag, uint32& OutAlign)
{
	//ZYZ_TODO:CalculateMipMap Support Later
	return 0;
}

uint64 FD3D11DynamicRHI::RHICalcTexture3DPlatformSize(uint32 SizeX, uint32 SizeY, uint32 SizeZ, uint8 Format, uint32 NumMips, uint32 Flag, uint32& OutAlign)
{
	//ZYZ_TODO:CalculateMipMap Support Later
	return 0;
}

uint64 FD3D11DynamicRHI::RHICalcTextureCubePlatformSize(uint32 Size, uint8 Format, uint32 NumMips, uint32 Flag, uint32& OutAlign)
{
	//ZYZ_TODO:CalculateMipMap Support Later
	return 0;
}

bool FD3D11DynamicRHI::RHIGetTextureMemoryVisualizeData(FColor* TextureData, int32 SizeX, int32 SizeY, int32 Pitch, int32 PixelSize)
{
	//Unreal Alsp didn't support
	return false;
}

void FD3D11DynamicRHI::RHIGenerateMips(FRHITexture* Texture)
{
	FD3D11TextureBase* TextureBase = (FD3D11TextureBase*)Texture;

	Direct3DDeviceIMContext->GenerateMips(TextureBase->GetShaderResourceView());

}

uint32 FD3D11DynamicRHI::RHIComputeMemorySize(FRHITexture* Texture)
{
	FD3D11TextureBase* TextureBase = (FD3D11TextureBase*)Texture;
	
	return TextureBase->GetMemorySize();
}

ETextureReallocationStatus FD3D11DynamicRHI::RHIFinalizeAsyncReallocateTexture2D(FRHITexture2D* Texture2D, bool bBlockUntilCompleted)
{
	return TexRealloc_Succeeded;
}

ETextureReallocationStatus FD3D11DynamicRHI::RHICancelAsyncReallocateTexture2D(FRHITexture2D* Texture2D, bool bBlockUntilCompleted)
{
	return TexRealloc_Succeeded;
}

void* FD3D11DynamicRHI::RHILockTexture2D(FRHITexture2D* Texture, uint32 MipIndex, EResourceLockMode LockMode, uint32& DestStride, bool bLockWithinMiptail)
{
	FD3D11Texture2D* D3D11Texture = (FD3D11Texture2D*)Texture;
	//ZYZ_TODO:Why Need to Clear SRV,Maybe didn't want someothers sample it.
	//ConditionalClearShaderResource(Texture);
	return D3D11Texture->Lock(MipIndex, 0, LockMode, DestStride);
}

void FD3D11DynamicRHI::RHIUnlockTexture2D(FRHITexture2D* Texture, uint32 MipIndex, bool bLockWithinMiptail)
{
	FD3D11Texture2D* D3D11Texture = (FD3D11Texture2D*)Texture;
	D3D11Texture->Unlock(MipIndex , 0);
}

void* FD3D11DynamicRHI::RHILockTexture2DArray(FRHITexture2DArray* TextureArray, uint32 TextureIndex, uint32 MipIndex, EResourceLockMode LockMode, uint32& DestStride, bool bLockWithInMiptail)
{
	FD3D11Texture2DArray* D3D11TextureArray = (FD3D11Texture2DArray*)TextureArray;
	//ZYZ_TODO:Why Need to Clear SRV,Maybe didn't want someothers sample it.
	//ConditionalClearShaderResource(D3D11TextureArray);
	return D3D11TextureArray->Lock(MipIndex, TextureIndex, LockMode, DestStride);
}

void FD3D11DynamicRHI::RHIUnlockTexture2DArray(FRHITexture2DArray* TextureArray, uint32 TextureIndex, uint32 MipIndex, bool bLockWithMiptail)
{
	FD3D11Texture2DArray* D3D11TextureArray = (FD3D11Texture2DArray*)TextureArray;
	D3D11TextureArray->Unlock(MipIndex, TextureIndex);
}

void* FD3D11DynamicRHI::RHILockTextureCubeFace(FRHITextureCube* TextureCube, uint32 FaceIndex, uint32 ArrayIndex, uint32 MipIndex, EResourceLockMode LockMode, uint32& DestStride, bool bLockWithInMiptail)
{
	FD3D11TextureCube* D3D11TextureCube = (FD3D11TextureCube*)TextureCube;
	//ZYZ_TODO:Why Need to Clear SRV,Maybe didn't want someothers sample it.
	//ConditionalClearShaderResource(D3D11TextureArray);
	uint32 D3DFace = GetD3D11CubeFace((ECubeFace)FaceIndex);
	return D3D11TextureCube->Lock(MipIndex , D3DFace+ArrayIndex*6 , LockMode , DestStride);
}

void FD3D11DynamicRHI::RHIUnLockTextureCubeFace(FRHITextureCube* TextureCube, uint32 FaceIndex, uint32 ArrayIndex, uint32 MipIndex, bool bLockWithinMiptail)
{
	FD3D11TextureCube* D3D11TextureCube = (FD3D11TextureCube*)TextureCube;
	uint32 D3DFace = GetD3D11CubeFace((ECubeFace)FaceIndex);
	D3D11TextureCube->Unlock(MipIndex, D3DFace + ArrayIndex * 6);
}

void FD3D11DynamicRHI::RHIUpdateTexture2D(FRHITexture2D* Texture2D, uint32 MipLevel, FUpdateTextureRegion2D& Region2D, uint32 SourcePitch, uint8* SourceData)
{
	FD3D11Texture2D* D11Texture2D = (FD3D11Texture2D*)Texture2D;
	D3D11_BOX box = {
		Region2D.DestX , Region2D.DestX + Region2D.Wight,0,
		Region2D.DestY , Region2D.DestY + Region2D.Height,1
	};

	Direct3DDeviceIMContext->UpdateSubresource(D11Texture2D->GetResource() ,MipLevel,&box, SourceData, SourcePitch , 0);
}

void FD3D11DynamicRHI::RHIUpdateTexture3D(FRHITexture3D* Texture3D, uint32 MipLevel, FUpdateTextureRegion3D& Region3D, uint32 SourceRowPitch, uint32 SourceDepthPitch, uint8* SourceData)
{
	FD3D11Texture3D* D11Texture3D = (FD3D11Texture3D*)Texture3D;

	D3D11_BOX box = {
		Region3D.DestX ,Region3D.DestY,Region3D.DestZ,
		Region3D.DestX + Region3D.Wight,Region3D.DestY + Region3D.Height , Region3D.DestZ + Region3D.Depth
	};

	Direct3DDeviceIMContext->UpdateSubresource(D11Texture3D->GetResource(), MipLevel, &box, SourceData, SourceRowPitch, SourceDepthPitch);
}


template <class RHIResourceType>
void* TD3D11Texture2D<RHIResourceType>::Lock(uint32 MipIndex, uint32 ArrayIndex, EResourceLockMode LockMode, uint32& DestStride)
{
	//Calculate Subresource
	uint32 Subresource = D3D11CalcSubresource(MipIndex , ArrayIndex , GetNumMips());
	uint32 BlockSizeX = GPixelFormats[GetPixelFormat()].BlockSizeX;
	uint32 BlockSizeY = GPixelFormats[GetPixelFormat()].BlockSizeY;
	uint32 BlockBytes = GPixelFormats[GetPixelFormat()].BlockBytes;
	uint32 MipSizeX = FMath::Max(BlockSizeX, GetSizeX() >> MipIndex);
	uint32 MipSizeY = FMath::Max(BlockSizeY, GetSizeY() >> MipIndex);
	uint32 NumBlockX = (MipSizeX + BlockSizeX - 1) / BlockSizeX;	//BlockSize:Compress format.how many pixels compress to one block
	uint32 NumBlockY = (MipSizeY + BlockSizeY - 1) / BlockSizeY;
	uint32 NumBytes = NumBlockX * NumBlockY * BlockBytes;

	//Texture Default Usage is Default so can't write and read
	FD3D11LockedData LockedData;
	if (LockMode == RLM_WriteOnly)
	{
		LockedData.AllocData(NumBytes);
		LockedData.Pitch = NumBlockX * BlockBytes;
	}
	else
	{
		D3D11_TEXTURE2D_DESC Texture2DDesc;
		((ID3D11Texture2D*)GetResource())->GetDesc(&Texture2DDesc);
		Texture2DDesc.Width = MipSizeX;
		Texture2DDesc.Height = MipSizeY;
		Texture2DDesc.MipLevels = 1;
		Texture2DDesc.ArraySize = 1;
		Texture2DDesc.Usage = D3D11_USAGE_STAGING;
		Texture2DDesc.BindFlags = 0;
		Texture2DDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		Texture2DDesc.MiscFlags = 0;

		ID3D11Texture2D* Texture;
		D3DRHI->GetDevice()->CreateTexture2D(&Texture2DDesc, NULL, &Texture);
		LockedData.StagingResource = Texture;

		D3DRHI->GetContext()->CopySubresourceRegion(Texture , 0, 0, 0, 0, GetResource(), Subresource, NULL);

		D3D11_MAPPED_SUBRESOURCE MappedSubResource;
		D3DRHI->GetContext()->Map(Texture, 0, D3D11_MAP_READ, 0, &MappedSubResource);
		LockedData.SetData(MappedSubResource.pData);
		LockedData.Pitch = MappedSubResource.RowPitch;
	}

	FD3D11LockedKey LockedKey((ID3D11Texture2D*)GetResource() , Subresource);
	D3DRHI->GetOutstandingLocks().insert(std::make_pair(LockedKey, LockedData));
	return (void*)LockedData.GetData();
}

template <class RHIResourceType>
void TD3D11Texture2D<RHIResourceType>::Unlock(uint32 MipIndex, uint32 ArrayIndex)
{
	const uint32 Subresource = D3D11CalcSubresource(MipIndex, ArrayIndex, GetNumMips());

	FD3D11LockedKey LockedKey((ID3D11Texture2D*)GetResource() , Subresource);
	FD3D11LockedData* LockedData = &(D3DRHI->GetOutstandingLocks().find(LockedKey)->second);

	if (LockedData->StagingResource)
	{
		D3DRHI->GetContext()->Unmap(LockedData->StagingResource, 0);
	}
	else
	{
		D3DRHI->GetContext()->UpdateSubresource(GetResource(), Subresource, NULL, LockedData->GetData(), LockedData->Pitch, 0);
		LockedData->FreeData();
	}

	D3DRHI->GetOutstandingLocks().erase(LockedKey);
}