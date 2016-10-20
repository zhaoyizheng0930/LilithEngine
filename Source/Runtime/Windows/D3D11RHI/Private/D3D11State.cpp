#include "D3D11RHIPCH.h"
#include "WindowsD3D11DynamicRHI.h"
#include "D3D11Definitions.h"

static D3D11_TEXTURE_ADDRESS_MODE TranslateAddressMode(ESamplerAddressMode AddressMode)
{
	switch (AddressMode)
	{
	case AM_Clamp: return D3D11_TEXTURE_ADDRESS_CLAMP;
	case AM_Mirror: return D3D11_TEXTURE_ADDRESS_MIRROR;
	case AM_Border: return D3D11_TEXTURE_ADDRESS_BORDER;
	default: return D3D11_TEXTURE_ADDRESS_WRAP;
	};
}


FRHISamplerState* FD3D11DynamicRHI::RHICreateSamplerState(const FSamplerStateInitializerRHI& Initializer)
{
	FD3D11SamplerState* SamplerState = nullptr;
	if (Direct3DDevice)
	{
		bool bComparisonEnable = Initializer.SamplerComparisonFunction != SCF_Never;
		D3D11_SAMPLER_DESC desc;
		switch (Initializer.Filter)
		{
		case SF_Point:
			desc.Filter = bComparisonEnable ? D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT : D3D11_FILTER_MIN_MAG_MIP_POINT;
			break;
		case SF_Bilinear:
			desc.Filter = bComparisonEnable ? D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT : D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
			break;
		case SF_Trilinear:
			desc.Filter = bComparisonEnable ? D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			break;
		case SF_AnisotropicPoint:
		case SF_AnisotropicLinear:
			if (Initializer.MaxAnisotropy == 1)
			{
				desc.Filter = bComparisonEnable ? D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			}
			else
			{
				// D3D11 doesn't allow using point filtering for mip filter when using anisotropic filtering
				desc.Filter = bComparisonEnable ? D3D11_FILTER_COMPARISON_ANISOTROPIC : D3D11_FILTER_ANISOTROPIC;
			}
			break;
		default:
			break;
		}
		desc.AddressU = TranslateAddressMode(Initializer.AddressU);
		desc.AddressV = TranslateAddressMode(Initializer.AddressV);
		desc.AddressW = TranslateAddressMode(Initializer.AddressW);
		desc.MipLODBias = Initializer.MipBias;
		desc.MaxAnisotropy = FMath::Clamp(Initializer.MaxAnisotropy, (int32)0, (int32)16);
		desc.MinLOD = Initializer.MinMipLevel;
		desc.MaxLOD = Initializer.MaxMipLevel;
		FColor color(Initializer.BorderColor);
		desc.BorderColor[0] = color.R;
		desc.BorderColor[1] = color.G;
		desc.BorderColor[2] = color.B;
		desc.BorderColor[3] = color.A;
		switch (Initializer.SamplerComparisonFunction)
		{
		case SCF_Less:
			desc.ComparisonFunc = D3D11_COMPARISON_LESS;
			break;
		case SCF_Never:
		default:
			desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			break;
		}

		ID3D11SamplerState* Sampler;
		Direct3DDevice->CreateSamplerState(&desc, &Sampler);
		SamplerState = new FD3D11SamplerState;
		SamplerState->Resource = Sampler;
		//Cache Sampler State Later
	}
	return SamplerState;
}

static D3D11_FILL_MODE TranslateFillMode(ERasterizerFillMode mode)
{
	switch (mode)
	{
	case FM_Point:
	case FM_Wireframe:
		return D3D11_FILL_WIREFRAME;
	case FM_Solid:
	default:
		return D3D11_FILL_SOLID;
	}
}

static D3D11_CULL_MODE TranslateCullMode(ERasterizerCullMode mode)
{
	switch (mode)
	{
	case CM_None:
		return D3D11_CULL_NONE;
	case CM_CW:
		return D3D11_CULL_BACK;
	case CM_CCW:
		return D3D11_CULL_FRONT;
	default:
	}
}

FRHIRasterizerState* FD3D11DynamicRHI::RHICreateRasterizerState(const FRasterizerStateInitializerRHI& Initializer)
{
	FD3D11RasterizerState* RHIRasterizerState = nullptr;
	if (Direct3DDevice)
	{
		D3D11_RASTERIZER_DESC desc;
		desc.FillMode = TranslateFillMode(Initializer.FillMode);
		desc.CullMode = TranslateCullMode(Initializer.CullMode);
		desc.FrontCounterClockwise = true; //true ÊÇÄæÊ±Õë³¯Ç°
		desc.DepthBias = FMath::FloorToInt(Initializer.DepthBias);
		desc.SlopeScaledDepthBias = Initializer.SlopeScaleDepthBias;
		desc.DepthClipEnable = true;
		desc.ScissorEnable = true;

		ID3D11RasterizerState* RasterizerState;
		Direct3DDevice->CreateRasterizerState(&desc, &RasterizerState);

		RHIRasterizerState = new FD3D11RasterizerState;
		RHIRasterizerState->Resource = RasterizerState;
	}
	return RHIRasterizerState;
}

static D3D11_COMPARISON_FUNC TranslateComparisonFunc(ECompareFunction InCompareFunc)
{
	switch (InCompareFunc)
	{
	case CF_Less:
		return D3D11_COMPARISON_LESS;
	case CF_LessEqual:
		return D3D11_COMPARISON_LESS_EQUAL;
	case CF_Greater:
		return D3D11_COMPARISON_GREATER;
	case CF_GreaterEqual:
		return D3D11_COMPARISON_GREATER_EQUAL;
	case CF_Equal:
		return D3D11_COMPARISON_EQUAL;
	case CF_NotEqual:
		return D3D11_COMPARISON_NOT_EQUAL;
	case CF_Never:
		return D3D11_COMPARISON_NEVER;
	case CF_Always:
		return D3D11_COMPARISON_ALWAYS;
	default:
		return D3D11_COMPARISON_ALWAYS;
		break;
	}
}

static D3D11_STENCIL_OP TranslateStencilOP(EStencilOp InStencilOp)
{
	switch (InStencilOp)
	{
	case SO_Keep:
		return D3D11_STENCIL_OP_KEEP;
	case SO_Zero:
		return D3D11_STENCIL_OP_ZERO;
	case SO_Replace:
		return D3D11_STENCIL_OP_REPLACE;
	case SO_SaturatedIncrement:
		return D3D11_STENCIL_OP_INCR_SAT;
	case SO_SaturatedDecrement:
		return D3D11_STENCIL_OP_DECR_SAT;
	case SO_Invert:
		return D3D11_STENCIL_OP_INVERT;
	case SO_Increment:
		return D3D11_STENCIL_OP_INCR;
	case SO_Decrement:
		return D3D11_STENCIL_OP_DECR;
	default:
		break;
	}
}

FRHIDepthStencilState* FD3D11DynamicRHI::RHICreateDepthStencilState(const FDepthStencilStateInitializerRHI& Initializer)
{
	FD3D11DepthStencilState* RHIDepthStencilState = nullptr;
	if (Direct3DDevice)
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		// depth part
		desc.DepthEnable = Initializer.DepthTest != CF_Always || Initializer.bEnableDepthWrite;
		desc.DepthWriteMask = Initializer.bEnableDepthWrite ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
		desc.DepthFunc = TranslateComparisonFunc(Initializer.DepthTest);

		//Stencil part
		desc.StencilEnable = Initializer.bEnableBackFaceStencil || Initializer.bEnableFrontFaceStencil;
		desc.StencilReadMask = Initializer.StencilReadMask;
		desc.StencilWriteMask = Initializer.StencilWriteMask;
		desc.FrontFace.StencilFailOp = TranslateStencilOP(Initializer.FrontFaceStencilFailStencilOp);
		desc.FrontFace.StencilDepthFailOp = TranslateStencilOP(Initializer.FrontFaceDepthFailStencilOp);
		desc.FrontFace.StencilPassOp = TranslateStencilOP(Initializer.FrontFacePassStencilOp);
		desc.FrontFace.StencilFunc = TranslateComparisonFunc(Initializer.FrontFaceDepthTest);
		if (Initializer.bEnableBackFaceStencil)
		{
			desc.BackFace.StencilFailOp = TranslateStencilOP(Initializer.BackFaceStencilFailStencilOp);
			desc.BackFace.StencilDepthFailOp = TranslateStencilOP(Initializer.BackFaceDepthFailStencilOp);
			desc.BackFace.StencilPassOp = TranslateStencilOP(Initializer.BackFacePassStencilOp);
			desc.BackFace.StencilFunc = TranslateComparisonFunc(Initializer.BackFaceDepthTest);
		}
		else
		{
			desc.BackFace = desc.FrontFace;
		}
		ID3D11DepthStencilState* DepthStencilState = nullptr;
		Direct3DDevice->CreateDepthStencilState(&desc , &DepthStencilState);
		RHIDepthStencilState = new FD3D11DepthStencilState;
		RHIDepthStencilState->Resource = DepthStencilState;
	}
	return RHIDepthStencilState;
}

FRHIBlendState* FD3D11DynamicRHI::RHICreateBlendState(const FBlendStateInitializerRHI& Initializer)
{
	FD3D11BlendState* RHIBlendState = nullptr;
	if (Direct3DDevice)
	{
		D3D11_BLEND_DESC BlendStateDesc;
		BlendStateDesc.RenderTarget;
		BOOL AlphaToCoverageEnable;
		BOOL IndependentBlendEnable;
		D3D11_RENDER_TARGET_BLEND_DESC RenderTarget[8];

		ID3D11BlendState* BlendState = nullptr;
		Direct3DDevice->CreateBlendState(&BlendStateDesc, &BlendState);
		RHIBlendState = new FD3D11BlendState;
		RHIBlendState->Resource = BlendState;

	}
}