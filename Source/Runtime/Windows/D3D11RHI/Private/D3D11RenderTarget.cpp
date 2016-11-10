#include "D3D11RHIPCH.h"
#include "WindowsD3D11DynamicRHI.h"
#include "RenderUtils.h"
#include "RHIResource.h"

static inline DXGI_FORMAT ConvertTypelessToUnorm(DXGI_FORMAT Format)
{
	// required to prevent 
	// D3D11: ERROR: ID3D11DeviceContext::ResolveSubresource: The Format (0x1b, R8G8B8A8_TYPELESS) is never able to resolve multisampled resources. [ RESOURCE_MANIPULATION ERROR #294: DEVICE_RESOLVESUBRESOURCE_FORMAT_INVALID ]
	// D3D11: **BREAK** enabled for the previous D3D11 message, which was: [ RESOURCE_MANIPULATION ERROR #294: DEVICE_RESOLVESUBRESOURCE_FORMAT_INVALID ]
	switch (Format)
	{
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
		return DXGI_FORMAT_R8G8B8A8_UNORM;

	case DXGI_FORMAT_B8G8R8A8_TYPELESS:
		return DXGI_FORMAT_B8G8R8A8_UNORM;

	default:
		return Format;
	}
}

void FD3D11DynamicRHI::RHICopyToResolveTarget(FRHITexture* SourceTexture, FRHITexture* DestTexture, bool bKeepOriginalSurface, const FResolveParams& ResolveParam)
{
	FD3D11Texture2D* SourceTexture2D = static_cast<FD3D11Texture2D*>(SourceTexture->GetTexture2D());
	FD3D11Texture2D* DestTexture2D = static_cast<FD3D11Texture2D*>(DestTexture->GetTexture2D());

	FD3D11TextureCube* SourceTextureCube = static_cast<FD3D11TextureCube*>(SourceTexture->GetTextureCube());
	FD3D11TextureCube* DestTextureCube = static_cast<FD3D11TextureCube*>(DestTexture->GetTextureCube());

	FD3D11Texture3D* SourceTexture3D = static_cast<FD3D11Texture3D*>(SourceTexture->GetTexture3D());
	FD3D11Texture3D* DestTexture3D = static_cast<FD3D11Texture3D*>(DestTexture->GetTexture3D());

	if (SourceTexture2D && DestTexture2D)
	{
		if (FeatureLevel == D3D_FEATURE_LEVEL_11_0 && DestTexture2D->GetDepthStencilView(FExclusiveDepthStencil::DepthWrite_StencilWrite)
			&&SourceTexture2D->GetNumSamples() > 1 && DestTexture2D->GetNumSamples() <= 1)
		{
			D3D11_TEXTURE2D_DESC desc;
			DestTexture2D->GetResource()->GetDesc(&desc);

			//Wait for Shader
			//ResolveTextureUsingShader<>();
		}
		else
		{
			DXGI_FORMAT SrcFmt = (DXGI_FORMAT)GPixelFormats[SourceTexture2D->GetPixelFormat()].PlatformFormat;
			DXGI_FORMAT DestFmt = (DXGI_FORMAT)GPixelFormats[DestTexture2D->GetPixelFormat()].PlatformFormat;

			DXGI_FORMAT Fmt = ConvertTypelessToUnorm((DXGI_FORMAT)GPixelFormats[DestTexture2D->GetPixelFormat()].PlatformFormat);

			if (SourceTexture2D->GetNumSamples() > 1 && DestTexture2D->GetNumSamples() <= 1)
			{
				Direct3DDeviceIMContext->ResolveSubresource(
					DestTexture2D->GetResource(),
					0,
					SourceTexture2D->GetResource(),
					0,
					Fmt
					);
			}
			else
			{
				if (ResolveParam.ResolveRect.IsValid())
				{
					D3D11_BOX SrcBox;

					SrcBox.left = ResolveParam.ResolveRect.X1;
					SrcBox.top = ResolveParam.ResolveRect.Y1;
					SrcBox.front = 0;
					SrcBox.right = ResolveParam.ResolveRect.X2;
					SrcBox.bottom = ResolveParam.ResolveRect.Y2;
					SrcBox.back = 1;

					Direct3DDeviceIMContext->CopySubresourceRegion(DestTexture2D->GetResource(), 0, ResolveParam.ResolveRect.X1, ResolveParam.ResolveRect.Y1, 0, SourceTexture2D->GetResource(), 0, &SrcBox);
				}
				else
				{
					Direct3DDeviceIMContext->CopyResource(DestTexture2D->GetResource(), SourceTexture2D->GetResource());
				}
			}
		}
	}
	else if (SourceTextureCube && DestTextureCube)
	{
		if (SourceTextureCube != DestTextureCube)
		{
			uint32 D3DFace = GetD3D11CubeFace(ResolveParam.CubeFace);
			uint32 SourceSubresource = D3D11CalcSubresource(ResolveParam.MipIndex ,ResolveParam.SourceArrayIndex * 6 + D3DFace, SourceTextureCube->GetNumMips());
			uint32 DestSubresource = D3D11CalcSubresource(ResolveParam.MipIndex, ResolveParam.DestArrayIndex * 6 + D3DFace, DestTextureCube->GetNumMips());

			//MSAA Resolve
			if (SourceTextureCube->GetNumSamples() > 1 && DestTextureCube->GetNumSamples() <= 1)
			{
				Direct3DDeviceIMContext->ResolveSubresource(DestTextureCube->GetResource(), DestSubresource, SourceTextureCube->GetResource(), SourceSubresource, (DXGI_FORMAT)GPixelFormats[DestTextureCube->GetPixelFormat()].PlatformFormat);
			}
			else
			{
				if (ResolveParam.ResolveRect.IsValid())
				{
					D3D11_BOX SrcBox;
					SrcBox.left = ResolveParam.ResolveRect.X1;
					SrcBox.right = ResolveParam.ResolveRect.X2;
					SrcBox.top = ResolveParam.ResolveRect.Y1;
					SrcBox.bottom = ResolveParam.ResolveRect.Y2;
					SrcBox.front = 0;
					SrcBox.back = 1;

					Direct3DDeviceIMContext->CopySubresourceRegion(DestTextureCube->GetResource(), DestSubresource, 0, 0, 0, SourceTextureCube->GetResource(), SourceSubresource, &SrcBox);
				}
				else
				{
					Direct3DDeviceIMContext->CopySubresourceRegion(DestTextureCube->GetResource(), DestSubresource, 0, 0, 0, SourceTextureCube->GetResource(), SourceSubresource, NULL);
				}
			}
		}
	}
	else if(SourceTexture2D && DestTextureCube)
	{
		uint32 D3DFace = GetD3D11CubeFace(ResolveParam.CubeFace);
		uint32 Subresource = D3D11CalcSubresource(0, D3DFace, 1);
		Direct3DDeviceIMContext->CopySubresourceRegion(DestTextureCube->GetResource() , Subresource ,0,0,0,SourceTexture2D->GetResource(),0,NULL );
	}
	else if (SourceTexture3D && DestTexture3D)
	{
		//NULL 
	}
}