#include "D3D11RHIPCH.h"
#include "WindowsD3D11DynamicRHI.h"
#include "RHIUtilities.h"

void FD3D11DynamicRHI::RHISubmitCommandsHint()
{

}

void FD3D11DynamicRHI::RHISetStreamSource(uint32 StreamIndex, FRHIVertexBuffer* VertexBuffer, uint32 Stride, uint32 Offset)
{
	FD3D11VertexBuffer* D11VertexBuffer = (FD3D11VertexBuffer*)VertexBuffer;
	StateCache.SetStreamSource(D11VertexBuffer->Resource , StreamIndex , Stride , Offset);
}

void FD3D11DynamicRHI::RHISetRasterizerState(FRHIRasterizerState* RasterizerState)
{
	FD3D11RasterizerState* D11RasterizerState = (FD3D11RasterizerState*)RasterizerState;
	StateCache.SetRasterizerState(D11RasterizerState->Resource);
}

void FD3D11DynamicRHI::RHISetScissorRect(bool bEnable, uint32 MinX, uint32 MinY, uint32 MaxX, uint32 MaxY)
{
	if (bEnable)
	{
		D3D11_RECT rect;
		rect.bottom = MaxY;
		rect.top = MinY;
		rect.left = MinX;
		rect.right = MaxX;
		Direct3DDeviceIMContext->RSSetScissorRects(1, &rect);
	}
	else
	{
		D3D11_RECT rect;
		rect.bottom = MaxY;
		rect.top = 0;
		rect.left = 0;
		rect.right = MaxX;
		Direct3DDeviceIMContext->RSSetScissorRects(1, &rect);
	}
}

void FD3D11DynamicRHI::RHISetDepthStencilState(FRHIDepthStencilState* NewState, uint32 StencilRef)
{
	FD3D11DepthStencilState* D11DSV = (FD3D11DepthStencilState*)NewState;

	//ZYZ_TODO:Don't know why.
	//ValidateExclusiveDepthStencilAccess(NewState->AccessType);

	StateCache.SetDepthStencilState(D11DSV->Resource , StencilRef);
}

void FD3D11DynamicRHI::RHISetBlendState(FRHIBlendState* NewState, const FLinearColor& BlendFactor)
{
	FD3D11BlendState* D11BlendState = (FD3D11BlendState*)NewState;

	StateCache.SetBlendState(D11BlendState->Resource , &BlendFactor , 0xffffffff);
}

void FD3D11DynamicRHI::RHISetViewport(uint32 MinX, uint32 MinY, float MinZ, uint32 MaxX, uint32 MaxY, float MaxZ)
{
	if (MinX <= D3D11_VIEWPORT_BOUNDS_MAX && MinY <= D3D11_VIEWPORT_BOUNDS_MAX && MaxX <= D3D11_VIEWPORT_BOUNDS_MAX && MaxY <= D3D11_VIEWPORT_BOUNDS_MAX)
	{
		D3D11_VIEWPORT Viewport = { (float)MinX, (float)MinY, (float)MinZ, (float)MaxX, (float)MaxY, (float)MaxZ };
		if (Viewport.Width > 0 && Viewport.Height > 0)
		{
			StateCache.SetViewport(Viewport);
			//SetScissorRectIfRequiredWhenSettingViewport(MinX, MinY, MaxX, MaxY);
		}
	}
}

void FD3D11DynamicRHI::RHISetStereoViewport(uint32 LeftMinX, uint32 RightMinX, uint32 MinY, float MinZ, uint32 LeftMaxX, uint32 RightMaxX, uint32 MaxY, float MaxZ)
{

}

void FD3D11DynamicRHI::RHISetBoundShaderState(FRHIBoundShaderState* BoundShaderState)
{
	//ZYZ_TODO:Don't know why....
	FD3D11BoundShaderState* D11BoundShaderState = (FD3D11BoundShaderState*)BoundShaderState;

	StateCache.SetInputLayout(D11BoundShaderState->InputeLayout);
	StateCache.SetVertexShader(D11BoundShaderState->VertexShader);
	StateCache.SetHullSahder(D11BoundShaderState->HullShader);
	StateCache.SetDomainShader(D11BoundShaderState->DomainShader);
	StateCache.SetGeometryShader(D11BoundShaderState->GeometryShader);
	StateCache.SetPixelShader(D11BoundShaderState->PixelShader);

	//What are Uniform Buffer Use for!!
}

void FD3D11DynamicRHI::RHISetShaderTexture(FRHIVertexShader* VertexShader, uint32 TextureIndex, FRHITexture* NewTexture)
{
	FD3D11VertexShader* D11VertexShader = (FD3D11VertexShader*)VertexShader;
	FD3D11TextureBase* D11NewTexture = (FD3D11Texture*)NewTexture;

	if (D11NewTexture == NULL || D11NewTexture->GetRenderTargetView(0,0) || D11NewTexture->HasDepthStencilView())
	{
		//if this texture use for RTV or SRV. it can't be a SRV. Unreal said it can be a dynamic StateCache,Why?
		assert(0);
	}
	else
	{
		SetShaderResourceView<SF_Vertex>(D11NewTexture , D11NewTexture->GetShaderResourceView() , TextureIndex , "");
	}
}

void FD3D11DynamicRHI::RHISetShaderTexture(FRHIHullShader* HullShader, uint32 TextureIndex, FRHITexture* NewTexture)
{
	FD3D11HullShader* D11HullShader = (FD3D11HullShader*)HullShader;
	FD3D11TextureBase* D11NewTexture = (FD3D11Texture*)NewTexture;

	if (D11NewTexture == NULL || D11NewTexture->GetRenderTargetView(0, 0) || D11NewTexture->HasDepthStencilView())
	{
		//if this texture use for RTV or SRV. it can't be a SRV. Unreal said it can be a dynamic StateCache,Why?
		assert(0);
	}
	else
	{
		SetShaderResourceView<SF_Hull>(D11NewTexture, D11NewTexture->GetShaderResourceView(), TextureIndex, "");
	}
}

void FD3D11DynamicRHI::RHISetShaderTexture(FRHIDomainShader* DomainShader, uint32 TextureIndex, FRHITexture* NewTexture)
{
	FD3D11HullShader* D11DomainShader = (FD3D11HullShader*)DomainShader;
	FD3D11TextureBase* D11NewTexture = (FD3D11Texture*)NewTexture;

	if (D11NewTexture == NULL || D11NewTexture->GetRenderTargetView(0, 0) || D11NewTexture->HasDepthStencilView())
	{
		//if this texture use for RTV or SRV. it can't be a SRV. Unreal said it can be a dynamic StateCache,Why?
		assert(0);
	}
	else
	{
		SetShaderResourceView<SF_Domain>(D11NewTexture, D11NewTexture->GetShaderResourceView(), TextureIndex, "");
	}
}

void FD3D11DynamicRHI::RHISetShaderTexture(FRHIGeometryShader* GeometryShader, uint32 TextureIndex, FRHITexture* NewTexture)
{
	FD3D11HullShader* D11GeometryShader = (FD3D11HullShader*)GeometryShader;
	FD3D11TextureBase* D11NewTexture = (FD3D11Texture*)NewTexture;

	if (D11NewTexture == NULL || D11NewTexture->GetRenderTargetView(0, 0) || D11NewTexture->HasDepthStencilView())
	{
		//if this texture use for RTV or SRV. it can't be a SRV. Unreal said it can be a dynamic StateCache,Why?
		assert(0);
	}
	else
	{
		SetShaderResourceView<SF_Geometry>(D11NewTexture, D11NewTexture->GetShaderResourceView(), TextureIndex, "");
	}
}

void FD3D11DynamicRHI::RHISetShaderTexture(FRHIComputeShader* ComputeShader, uint32 TextureIndex, FRHITexture* NewTexture)
{
	FD3D11ComputeShader* D11ComputeShader = (FD3D11ComputeShader*)ComputeShader;
	FD3D11TextureBase* D11NewTexture = (FD3D11Texture*)NewTexture;

	if (D11NewTexture == NULL || D11NewTexture->GetRenderTargetView(0, 0) || D11NewTexture->HasDepthStencilView())
	{
		//if this texture use for RTV or SRV. it can't be a SRV. Unreal said it can be a dynamic StateCache,Why?
		assert(0);
	}
	else
	{
		SetShaderResourceView<SF_Compute>(D11NewTexture, D11NewTexture->GetShaderResourceView(), TextureIndex, "");
	}
}

void FD3D11DynamicRHI::RHISetShaderTexture(FRHIPixelShader* PixelShader, uint32 TextureIndex, FRHITexture* NewTexture)
{
	FD3D11PixelShader* D11ComputeShader = (FD3D11PixelShader*)PixelShader;
	FD3D11TextureBase* D11NewTexture = (FD3D11Texture*)NewTexture;

	if (D11NewTexture == NULL || D11NewTexture->GetRenderTargetView(0, 0) || D11NewTexture->HasDepthStencilView())
	{
		//if this texture use for RTV or SRV. it can't be a SRV. Unreal said it can be a dynamic StateCache,Why?
		assert(0);
	}
	else
	{
		SetShaderResourceView<SF_Pixel>(D11NewTexture, D11NewTexture->GetShaderResourceView(), TextureIndex, "");
	}
}

void FD3D11DynamicRHI::RHISetShaderSampler(FRHIVertexShader* VertexShader, uint32 SamplerIndex, FRHISamplerState* NewState)
{
	FD3D11VertexShader* D11VertexShader = (FD3D11VertexShader*)VertexShader;
	FD3D11SamplerState* D11NewState = (FD3D11SamplerState*)NewState;

	StateCache.SetSamplerState<SF_Vertex>(D11NewState->Resource , SamplerIndex);
}

void FD3D11DynamicRHI::RHISetShaderSampler(FRHIHullShader* HullShader, uint32 SamplerIndex, FRHISamplerState* NewState)
{
	FD3D11HullShader* D11HullShader = (FD3D11HullShader*)HullShader;
	FD3D11SamplerState* D11NewState = (FD3D11SamplerState*)NewState;

	StateCache.SetSamplerState<SF_Hull>(D11NewState->Resource, SamplerIndex);
}

void FD3D11DynamicRHI::RHISetShaderSampler(FRHIDomainShader* DomainShader, uint32 SamplerIndex, FRHISamplerState* NewState)
{
	FD3D11DomainShader* D11DomainShader = (FD3D11DomainShader*)DomainShader;
	FD3D11SamplerState* D11NewState = (FD3D11SamplerState*)NewState;

	StateCache.SetSamplerState<SF_Domain>(D11NewState->Resource, SamplerIndex);
}

void FD3D11DynamicRHI::RHISetShaderSampler(FRHIGeometryShader* GeometryShader, uint32 SamplerIndex, FRHISamplerState* NewState)
{
	FD3D11GeometryShader* D11GeometryShader = (FD3D11GeometryShader*)GeometryShader;
	FD3D11SamplerState* D11NewState = (FD3D11SamplerState*)NewState;

	StateCache.SetSamplerState<SF_Geometry>(D11NewState->Resource, SamplerIndex);
}

void FD3D11DynamicRHI::RHISetShaderSampler(FRHIComputeShader* ComputeShader, uint32 SamplerIndex, FRHISamplerState* NewState)
{
	FD3D11ComputeShader* D11ComputeShader = (FD3D11ComputeShader*)ComputeShader;
	FD3D11SamplerState* D11NewState = (FD3D11SamplerState*)NewState;

	StateCache.SetSamplerState<SF_Compute>(D11NewState->Resource, SamplerIndex);
}

void FD3D11DynamicRHI::RHISetShaderSampler(FRHIPixelShader* PixelShader, uint32 SamplerIndex, FRHISamplerState* NewState)
{
	FD3D11ComputeShader* D11PixelShader = (FD3D11ComputeShader*)PixelShader;
	FD3D11SamplerState* D11NewState = (FD3D11SamplerState*)NewState;

	StateCache.SetSamplerState<SF_Pixel>(D11NewState->Resource, SamplerIndex);
}

void FD3D11DynamicRHI::RHISetUAVParameter(FRHIComputeShader* ComputeShader, uint32 UAVIndex, FRHIUnorderedAccessView* UAV)
{
	FD3D11UnorderedAccessView* D11UAV = (FD3D11UnorderedAccessView*)UAV;

	if (D11UAV)
	{
		//ZYZ_TODO:Don't know why
		//ConditionalClearShaderResource(UAV->Resource);

		D11UAV->Resource->SetDirty(true , 0);
	}
	uint32 InitialCount = -1;
	Direct3DDeviceIMContext->CSSetUnorderedAccessViews(UAVIndex, 1, &(D11UAV->View), &InitialCount);
}

void FD3D11DynamicRHI::RHISetUAVParameter(FRHIComputeShader* ComputeShader, uint32 UAVIndex, FRHIUnorderedAccessView* UAV, uint32 InitialCount)
{
	FD3D11UnorderedAccessView* D11UAV = (FD3D11UnorderedAccessView*)UAV;

	if (D11UAV)
	{
		//ZYZ_TODO:Don't know why
		//ConditionalClearShaderResource(UAV->Resource);

		D11UAV->Resource->SetDirty(true, 0);
	}

	Direct3DDeviceIMContext->CSSetUnorderedAccessViews(UAVIndex, 1, &(D11UAV->View), &InitialCount);
}

void FD3D11DynamicRHI::RHISetShaderResourceViewParameter(FRHIVertexShader* VertexShader, uint32 SRVIndex, FRHIShaderResourceView* SRV)
{
	FD3D11ShaderResourceView* D11ShaderResource = (FD3D11ShaderResourceView*)SRV;
	SetShaderResourceView<SF_Pixel>(D11ShaderResource->Resource, D11ShaderResource->View , SRVIndex , "");
}

void FD3D11DynamicRHI::RHISetShaderResourceViewParameter(FRHIHullShader* HullShader, uint32 SRVIndex, FRHIShaderResourceView* SRV)
{
	FD3D11ShaderResourceView* D11ShaderResource = (FD3D11ShaderResourceView*)SRV;
	SetShaderResourceView<SF_Hull>(D11ShaderResource->Resource, D11ShaderResource->View, SRVIndex, "");
}

void FD3D11DynamicRHI::RHISetShaderResourceViewParameter(FRHIDomainShader* DomainShader, uint32 SRVIndex, FRHIShaderResourceView* SRV)
{
	FD3D11ShaderResourceView* D11ShaderResource = (FD3D11ShaderResourceView*)SRV;
	SetShaderResourceView<SF_Domain>(D11ShaderResource->Resource, D11ShaderResource->View, SRVIndex, "");
}

void FD3D11DynamicRHI::RHISetShaderResourceViewParameter(FRHIGeometryShader* GeometryShader, uint32 SRVIndex, FRHIShaderResourceView* SRV)
{
	FD3D11ShaderResourceView* D11ShaderResource = (FD3D11ShaderResourceView*)SRV;
	SetShaderResourceView<SF_Geometry>(D11ShaderResource->Resource, D11ShaderResource->View, SRVIndex, "");
}

void FD3D11DynamicRHI::RHISetShaderResourceViewParameter(FRHIComputeShader* ComputeShader, uint32 SRVIndex, FRHIShaderResourceView* SRV)
{
	FD3D11ShaderResourceView* D11ShaderResource = (FD3D11ShaderResourceView*)SRV;
	SetShaderResourceView<SF_Compute>(D11ShaderResource->Resource, D11ShaderResource->View, SRVIndex, "");
}

void FD3D11DynamicRHI::RHISetShaderResourceViewParameter(FRHIPixelShader* PixelShader, uint32 SRVIndex, FRHIShaderResourceView* SRV)
{
	FD3D11ShaderResourceView* D11ShaderResource = (FD3D11ShaderResourceView*)SRV;
	SetShaderResourceView<SF_Pixel>(D11ShaderResource->Resource, D11ShaderResource->View, SRVIndex, "");
}
struct FRTVDesc
{
	uint32 Width;
	uint32 Height;
	DXGI_SAMPLE_DESC SampleDesc;
};

FRTVDesc GetRenderTargetViewDesc(ID3D11RenderTargetView* RenderTargetView)
{
	D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
	RenderTargetView->GetDesc(&RTVDesc);
	FRTVDesc Result;
	uint32 MipIndex = 0;
	switch (RTVDesc.ViewDimension)
	{
	case D3D11_RTV_DIMENSION_TEXTURE1DARRAY:break;
	case D3D11_RTV_DIMENSION_TEXTURE2D:
	case D3D11_RTV_DIMENSION_TEXTURE2DARRAY:
	case D3D11_RTV_DIMENSION_TEXTURE2DMS:
	case D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY:
		ID3D11Texture2D* Texture2D = nullptr;
		RenderTargetView->GetResource((ID3D11Resource**)Texture2D);
		D3D11_TEXTURE2D_DESC Texture2DDesc;
		Texture2D->GetDesc(&Texture2DDesc);
		Result.Height = Texture2DDesc.Height;
		Result.Width = Texture2DDesc.Width;
		Result.SampleDesc = Texture2DDesc.SampleDesc;
		if (RTVDesc.ViewDimension == D3D11_RTV_DIMENSION_TEXTURE2D || RTVDesc.ViewDimension == D3D11_RTV_DIMENSION_TEXTURE2DARRAY)
		{
			MipIndex = RTVDesc.Texture2D.MipSlice;
		}
		break;
	case D3D11_RTV_DIMENSION_TEXTURE3D:break;
		ID3D11Texture3D* Texture3D = nullptr;
		RenderTargetView->GetResource((ID3D11Resource**)Texture3D);
		D3D11_TEXTURE3D_DESC Texture3DDesc;
		Texture3D->GetDesc(&Texture3DDesc);
		Result.Height = Texture3DDesc.Height;
		Result.Width = Texture3DDesc.Width;
		Result.SampleDesc.Count = 1;
		Result.SampleDesc.Quality = 0;
		MipIndex = RTVDesc.Texture3D.MipSlice;
	default:
		break;
	}

	Result.Height >>= MipIndex;
	Result.Width >>= MipIndex;
	return Result;
}

void FD3D11DynamicRHI::RHISetRenderTarget(uint32 NumSimulataneousRenderTargets, const FRHIRenderTargetView* NewRenderTarget, const FRHIDepthRenderTargetView* NewDepthStencilTarget, uint32 NumUAVs, FRHIUnorderedAccessView** UAVs)
{
	//DSV---------------------------------------------------------------------------------------------------------------
	FD3D11TextureBase* D11DepthStencilTarget = (FD3D11TextureBase*)(NewDepthStencilTarget->Texture->GetTextureBaseRHI());
	bool bTargetChanged = false;
	//ZYZ_TODO: Set the appropriate depth stencil view depending on whether depth writes are enabled or not
	ID3D11DepthStencilView* DSV;
	if (D11DepthStencilTarget)
	{
		CurrentDSVAccessType = NewDepthStencilTarget->GetDepthStencilAccess();
		DSV = D11DepthStencilTarget->GetDepthStencilView(CurrentDSVAccessType);

		//ZYZ_TODO:Don't know why
		//ConditionalClearShaderResource(NewDepthStencilTarget);
	}
	//Compare Dirty
	if (CurrentDepthStencilTarget != DSV)
	{
		CurrentDepthTexture = D11DepthStencilTarget;
		CurrentDepthStencilTarget = DSV;
		bTargetChanged = true;
	}

	//ZYZ_TODO:Set GPUAccess Dirty.Don't know what these used for
	if (D11DepthStencilTarget)
	{
		uint32 CurrentFrame = PresentCounter;
		const EResourceTransitionAccess CurrentAccess = D11DepthStencilTarget->GetCurrentGPUAccess();
		const uint32 LastFrameWritten = D11DepthStencilTarget->GetLastFrameWritten();
		const bool bReadable = CurrentAccess == EResourceTransitionAccess::EReadable;
		const bool bDepthWrite = NewDepthStencilTarget->GetDepthStencilAccess().IsDepthWrite();
		const bool bAccessValid = !bReadable ||
			LastFrameWritten != CurrentFrame ||
			!bDepthWrite;

		if (!bAccessValid || (bReadable && bDepthWrite))
		{
			D11DepthStencilTarget->SetCurrentGPUAccess(EResourceTransitionAccess::EWritable);
		}

		if (bDepthWrite)
		{
			D11DepthStencilTarget->SetDirty(true, CurrentFrame);
		}
	}

	//RTV---------------------------------------------------------------------------------------------------------------
	ID3D11RenderTargetView* NewRenderTargetView[MaxSimultaneousRenderTargets];
	for (int RenderTargetIndex = 0; RenderTargetIndex < MaxSimultaneousRenderTargets;RenderTargetIndex++)
	{
		ID3D11RenderTargetView* MipRendertargetView = NULL;
		if (RenderTargetIndex <NumSimulataneousRenderTargets && NewRenderTarget[RenderTargetIndex].Texture != nullptr)
		{
			FD3D11TextureBase* D11RenderTargetBase = (FD3D11TextureBase*)(NewRenderTarget[RenderTargetIndex].Texture->GetTextureBaseRHI());
			int32 RTMipLevel = NewRenderTarget[RenderTargetIndex].MipIndex;
			int32 RTSliceIndex = NewRenderTarget[RenderTargetIndex].ArraySliceIndex;
			MipRendertargetView = D11RenderTargetBase->GetRenderTargetView(RTMipLevel, RTSliceIndex);
			if (D11RenderTargetBase)
			{
				uint32 CurrentFrame = PresentCounter;
				const EResourceTransitionAccess CurrentAccess = D11RenderTargetBase->GetCurrentGPUAccess();
				const uint32 LastFrameWritten = D11RenderTargetBase->GetLastFrameWritten();
				const bool bReadable = CurrentAccess == EResourceTransitionAccess::EReadable;
				const bool bAccessValid = !bReadable || LastFrameWritten != CurrentFrame;

				if (!bAccessValid || bReadable)
				{
					D11RenderTargetBase->SetCurrentGPUAccess(EResourceTransitionAccess::EWritable);
				}
				D11RenderTargetBase->SetDirty(true, CurrentFrame);
			}

			// Unbind any shader views of the render target that are bound.
			//ConditionalClearShaderResource(NewRenderTarget);
		}

		NewRenderTargetView[RenderTargetIndex] = MipRendertargetView;

		//SetCache
		if (CurrentRenderTargets[RenderTargetIndex] != MipRendertargetView)
		{
			CurrentRenderTargets[RenderTargetIndex] = MipRendertargetView;
			bTargetChanged = true;
		}
	}

	//UAV---------------------------------------------------------------------------------------------------------
	for (uint32 UAVIndex = 0; UAVIndex < MaxSimultaneousUAVs; UAVIndex++)
	{
		ID3D11UnorderedAccessView* UAV = NULL;
		if (UAVIndex < NumUAVs && UAVs[UAVIndex] != NULL)
		{
			FD3D11UnorderedAccessView* RHIUAV = (FD3D11UnorderedAccessView*)UAVs[UAVIndex];
			UAV = RHIUAV->View;
			if (UAV)
			{
				const EResourceTransitionAccess CurrentUAVAccess = RHIUAV->Resource->GetCurrentGPUAccess();
				const bool UAVDirty = RHIUAV->Resource->IsDirty();
				const bool bAccessPass = (CurrentUAVAccess == EResourceTransitionAccess::ERWBarrier && !UAVDirty) || (CurrentUAVAccess == EResourceTransitionAccess::ERWNoBarrier);

				RHIUAV->Resource->SetDirty(true, PresentCounter);
			}

			// Unbind any shader views of the UAV's resource.
			//ConditionalClearShaderResource(RHIUAV->Resource);
		}

		if (CurrentUAVs[UAVIndex] != UAV)
		{
			CurrentUAVs[UAVIndex] = UAV;
			bTargetChanged = true;
		}
	}

	if (bTargetChanged)
	{
		CommitRenderTargetsAndUAVs();
	}

	//ViewPort Set to Full Size of render target 0
	if (NewRenderTargetView[0])
	{
		FRTVDesc desc = GetRenderTargetViewDesc(NewRenderTargetView[0]);
		RHISetViewport(0, 0, 0, desc.Width,desc.Height , 1);
	}
	else if (DSV)
	{
		ID3D11Texture2D* Texture;
		DSV->GetResource((ID3D11Resource**)Texture);
		D3D11_TEXTURE2D_DESC Texture2DDesc;
		Texture->GetDesc(&Texture2DDesc);
		RHISetViewport(0, 0, 0, Texture2DDesc.Width, Texture2DDesc.Height, 1.0f);
	}
}

void FD3D11DynamicRHI::RHISetRenderTargetAndClear(FRHISetRenderTargetsInfo* RenderTargetInfo)
{
	//ZYZ_TODO:Support later
}

void FD3D11DynamicRHI::RHISetShaderUniformBuffer(FRHIVertexShader* VertexShader, uint32 BufferIndex, FRHIUniformBuffer* Buffer)
{
	FD3D11UniformBuffer* D11Buffer = (FD3D11UniformBuffer*)Buffer;

	StateCache.SetConstantBuffer<SF_Vertex>(D11Buffer->Resource, BufferIndex);

	BoundUniformBuffers[SF_Vertex][BufferIndex] = D11Buffer;
	DirtyUniformBuffers[SF_Vertex] |= (1 << BufferIndex);
}

void FD3D11DynamicRHI::RHISetShaderUniformBuffer(FRHIHullShader* HullShader, uint32 BufferIndex, FRHIUniformBuffer* Buffer)
{
	FD3D11UniformBuffer* D11Buffer = (FD3D11UniformBuffer*)Buffer;

	StateCache.SetConstantBuffer<SF_Hull>(D11Buffer->Resource, BufferIndex);

	BoundUniformBuffers[SF_Hull][BufferIndex] = D11Buffer;
	DirtyUniformBuffers[SF_Hull] |= (1 << BufferIndex);
}

void FD3D11DynamicRHI::RHISetShaderUniformBuffer(FRHIDomainShader* DomainShader, uint32 BufferIndex, FRHIUniformBuffer* Buffer)
{
	FD3D11UniformBuffer* D11Buffer = (FD3D11UniformBuffer*)Buffer;

	StateCache.SetConstantBuffer<SF_Domain>(D11Buffer->Resource, BufferIndex);

	BoundUniformBuffers[SF_Domain][BufferIndex] = D11Buffer;
	DirtyUniformBuffers[SF_Domain] |= (1 << BufferIndex);
}

void FD3D11DynamicRHI::RHISetShaderUniformBuffer(FRHIGeometryShader* GeometryShader, uint32 BufferIndex, FRHIUniformBuffer* Buffer)
{
	FD3D11UniformBuffer* D11Buffer = (FD3D11UniformBuffer*)Buffer;

	StateCache.SetConstantBuffer<SF_Geometry>(D11Buffer->Resource, BufferIndex);

	BoundUniformBuffers[SF_Geometry][BufferIndex] = D11Buffer;
	DirtyUniformBuffers[SF_Geometry] |= (1 << BufferIndex);
}

void FD3D11DynamicRHI::RHISetShaderUniformBuffer(FRHIComputeShader* ComputeShader, uint32 BufferIndex, FRHIUniformBuffer* Buffer)
{
	FD3D11UniformBuffer* D11Buffer = (FD3D11UniformBuffer*)Buffer;

	StateCache.SetConstantBuffer<SF_Compute>(D11Buffer->Resource, BufferIndex);

	BoundUniformBuffers[SF_Compute][BufferIndex] = D11Buffer;
	DirtyUniformBuffers[SF_Compute] |= (1 << BufferIndex);
}

void FD3D11DynamicRHI::RHISetShaderUniformBuffer(FRHIPixelShader* PixelShader, uint32 BufferIndex, FRHIUniformBuffer* Buffer)
{
	FD3D11UniformBuffer* D11Buffer = (FD3D11UniformBuffer*)Buffer;

	StateCache.SetConstantBuffer<SF_Pixel>(D11Buffer->Resource, BufferIndex);

	BoundUniformBuffers[SF_Pixel][BufferIndex] = D11Buffer;
	DirtyUniformBuffers[SF_Pixel] |= (1 << BufferIndex);
}

void FD3D11DynamicRHI::RHISetShaderParameter(FRHIVertexShader* VertexShader, uint32 BufferIndex, uint32 BaseIndex, uint32 NumBytes, const void* NewValue)
{
	VSConstantBuffers[BufferIndex]->UpdateConstant((const uint8*)NewValue , BaseIndex , NumBytes);
}

void FD3D11DynamicRHI::RHISetShaderParameter(FRHIHullShader* HullShader, uint32 BufferIndex, uint32 BaseIndex, uint32 NumBytes, const void* NewValue)
{
	HSConstantBuffers[BufferIndex]->UpdateConstant((const uint8*)NewValue, BaseIndex, NumBytes);
}

void FD3D11DynamicRHI::RHISetShaderParameter(FRHIDomainShader* DomainShader, uint32 BufferIndex, uint32 BaseIndex, uint32 NumBytes, const void* NewValue)
{
	DSConstantBuffers[BufferIndex]->UpdateConstant((const uint8*)NewValue, BaseIndex, NumBytes);
}

void FD3D11DynamicRHI::RHISetShaderParameter(FRHIGeometryShader* GeometryShader, uint32 BufferIndex, uint32 BaseIndex, uint32 NumBytes, const void* NewValue)
{
	GSConstantBuffers[BufferIndex]->UpdateConstant((const uint8*)NewValue, BaseIndex, NumBytes);
}

void FD3D11DynamicRHI::RHISetShaderParameter(FRHIComputeShader* ComputeShader, uint32 BufferIndex, uint32 BaseIndex, uint32 NumBytes, const void* NewValue)
{
	CSConstantBuffers[BufferIndex]->UpdateConstant((const uint8*)NewValue, BaseIndex, NumBytes);
}

void FD3D11DynamicRHI::RHISetShaderParameter(FRHIPixelShader* PixelShader, uint32 BufferIndex, uint32 BaseIndex, uint32 NumBytes, const void* NewValue)
{
	PSConstantBuffers[BufferIndex]->UpdateConstant((const uint8*)NewValue, BaseIndex, NumBytes);
}

template <class ShaderType>
void FD3D11DynamicRHI::SetResourcesFromTables(const ShaderType* Shader)
{
	uint32 DirtyBits = Shader->ShaderResourceTable.ResourceTableBits & DirtyUniformBuffers[ShaderType::StaticFrequency];

	while (DirtyBits)
	{
		const uint32 LowestBitMask = (DirtyBits)& (-(int32)DirtyBits);
		const int32 BufferIndex = FMath::FloorLog2(LowestBitMask);
		DirtyBits ^= LowestBitMask;
		FD3D11UniformBuffer* Buffer = (FD3D11UniformBuffer*)BoundUniformBuffers[ShaderType::StaticFrequency][BufferIndex];

		SetShaderResourcesFromBuffer_Surface<(EShaderFrequency)ShaderType::StaticFrequency>(this, &StateCache, Buffer, Shader->ShaderResourceTable.TextureMap.GetData(), BufferIndex);
		SetShaderResourcesFromBuffer_SRV<(EShaderFrequency)ShaderType::StaticFrequency>(this, &StateCache, Buffer, Shader->ShaderResourceTable.TextureMap.GetData(), BufferIndex);
		SetShaderResourcesFromBuffer_Sampler<(EShaderFrequency)ShaderType::StaticFrequency>(this, &StateCache, Buffer, Shader->ShaderResourceTable.TextureMap.GetData(), BufferIndex);
	}
}

template <EShaderFrequency Frequency>
void SetResource(FD3D11DynamicRHI* D3D11RHI , FD3D11StateCache* StateCache , uint32 BindIndex , FD3D11BaseShaderResource* ShaderResource , FD3D11ShaderResourceView* SRV , std::string name = "")
{
	D3D11RHI->SetShaderResourceView<Frequency>(ShaderResource , SRV , BindIndex , name , FD3D11StateCache::SRV_Unknown);
}

template <EShaderFrequency Frequency>
void SetResource(FD3D11DynamicRHI* D3D11RHI, FD3D11StateCache* StateCache, uint32 BindIndex, ID3D11SamplerState* SamplerState)
{
	StateCache->SetSamplerState<Frequency>(SamplerState , BindIndex)
}

template <EShaderFrequency ShaderFrequency>
inline int32 SetShaderResourcesFromBuffer_Surface(FD3D11DynamicRHI* D3D11RHI, FD3D11StateCache* StateCache, FD3D11UniformBuffer* Buffer, const uint32* ResourceMap, int32 BufferIndex)
{

}

template <EShaderFrequency ShaderFrequency>
inline int32 SetShaderResourcesFromBuffer_SRV(FD3D11DynamicRHI* D3D11RHI, FD3D11StateCache* StateCache, FD3D11UniformBuffer* Buffer, const uint32* ResourceMap, int32 BufferIndex)
{

}

template <EShaderFrequency ShaderFrequency>
inline int32 SetShaderResourcesFromBuffer_Sampler(FD3D11DynamicRHI* D3D11RHI, FD3D11StateCache* StateCache, FD3D11UniformBuffer* Buffer, const uint32* ResourceMap, int32 BufferIndex)
{

}

static D3D11_PRIMITIVE_TOPOLOGY GetD3D11PrimitiveType(uint32 PrimitiveType, bool bUsingTessellation)
{
	if (bUsingTessellation)
	{
		switch (PrimitiveType)
		{
		case PT_1_ControlPointPatchList: return D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST;
		case PT_2_ControlPointPatchList: return D3D11_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST;

			// This is the case for tessellation without AEN or other buffers, so just flip to 3 CPs
		case PT_TriangleList: return D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST;

		case PT_LineList:
		case PT_TriangleStrip:
		case PT_QuadList:
		case PT_PointList:
			//UE_LOG(LogD3D11RHI, Fatal, TEXT("Invalid type specified for tessellated render, probably missing a case in FStaticMeshSceneProxy::GetMeshElement"));
			break;
		default:
			// Other cases are valid.
			break;
		};
	}

	switch (PrimitiveType)
	{
	case PT_TriangleList: return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	case PT_TriangleStrip: return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	case PT_LineList: return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	case PT_PointList: return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

		// ControlPointPatchList types will pretend to be TRIANGLELISTS with a stride of N 
		// (where N is the number of control points specified), so we can return them for
		// tessellation and non-tessellation. This functionality is only used when rendering a 
		// default material with something that claims to be tessellated, generally because the 
		// tessellation material failed to compile for some reason.
	case PT_3_ControlPointPatchList: return D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST;
	case PT_4_ControlPointPatchList: return D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST;
	case PT_5_ControlPointPatchList: return D3D11_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST;
	case PT_6_ControlPointPatchList: return D3D11_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST;
	case PT_7_ControlPointPatchList: return D3D11_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST;
	case PT_8_ControlPointPatchList: return D3D11_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST;
	case PT_9_ControlPointPatchList: return D3D11_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST;
	case PT_10_ControlPointPatchList: return D3D11_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST;
	case PT_11_ControlPointPatchList: return D3D11_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST;
	case PT_12_ControlPointPatchList: return D3D11_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST;
	case PT_13_ControlPointPatchList: return D3D11_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST;
	case PT_14_ControlPointPatchList: return D3D11_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST;
	case PT_15_ControlPointPatchList: return D3D11_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST;
	case PT_16_ControlPointPatchList: return D3D11_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST;
	case PT_17_ControlPointPatchList: return D3D11_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST;
	case PT_18_ControlPointPatchList: return D3D11_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST;
	case PT_19_ControlPointPatchList: return D3D11_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST;
	case PT_20_ControlPointPatchList: return D3D11_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST;
	case PT_21_ControlPointPatchList: return D3D11_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST;
	case PT_22_ControlPointPatchList: return D3D11_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST;
	case PT_23_ControlPointPatchList: return D3D11_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST;
	case PT_24_ControlPointPatchList: return D3D11_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST;
	case PT_25_ControlPointPatchList: return D3D11_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST;
	case PT_26_ControlPointPatchList: return D3D11_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST;
	case PT_27_ControlPointPatchList: return D3D11_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST;
	case PT_28_ControlPointPatchList: return D3D11_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST;
	case PT_29_ControlPointPatchList: return D3D11_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST;
	case PT_30_ControlPointPatchList: return D3D11_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST;
	case PT_31_ControlPointPatchList: return D3D11_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST;
	case PT_32_ControlPointPatchList: return D3D11_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST;
	//default: UE_LOG(LogD3D11RHI, Fatal, TEXT("Unknown primitive type: %u"), PrimitiveType);
	};

	return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

void FD3D11DynamicRHI::RHIDrawPrimitive(uint32 PrimitiveType, uint32 BaseVertexIndex, uint32 NumPrimitives, uint32 NumInstances)
{
	//ZYZ_TODO:CommitResource Uniform Buffer
	//CommitGraphicResourceTables();
	//ZYZ_TODO:CommitShaderConstants Uniform Buffer
	//CommitNonComputeShaderConstants();
	//PrimitiveTopology
	uint32 VertexCount = GetVertexCountForPrimitiveCount(NumPrimitives , PrimitiveType);
	StateCache.SetPrimitiveTopology(GetD3D11PrimitiveType(PrimitiveType , bUseTesslation));
	//DrawCall
	if (NumInstances > 1)
	{
		Direct3DDeviceIMContext->DrawInstanced(VertexCount , NumInstances , BaseVertexIndex , 0);
	}
	else
	{
		Direct3DDeviceIMContext->Draw(VertexCount , BaseVertexIndex);
	}
}

void FD3D11DynamicRHI::RHIDrawPrimitiveIndirect(uint32 PrimitiveType, FRHIVertexBuffer* ArgumentBuffer, uint32 ArgumentOffset)
{
	FD3D11VertexBuffer* D11Vertextbuffer = (FD3D11VertexBuffer*)ArgumentBuffer;
	//ZYZ_TODO:CommitResource Uniform Buffer
	//CommitGraphicResourceTables();
	//ZYZ_TODO:CommitShaderConstants Uniform Buffer
	//CommitNonComputeShaderConstants();
	StateCache.SetPrimitiveTopology(GetD3D11PrimitiveType(PrimitiveType, bUseTesslation));

	Direct3DDeviceIMContext->DrawInstancedIndirect(D11Vertextbuffer->Resource, ArgumentOffset);

}

void FD3D11DynamicRHI::RHIDrawIndexIndirect(FRHIIndexBuffer* IndexBuffer, uint32 PrimitiveType, FRHIStructureBuffer* StructureBuffer, int32 DrawArgumentIndex, uint32 NumInstances)
{
	FD3D11IndexBuffer* D11IndexBuffer = (FD3D11IndexBuffer*)IndexBuffer;
	FD3D11StructureBuffer* D11StructureBuffer = (FD3D11StructureBuffer*)StructureBuffer;

	//ZYZ_TODO:CommitResource Uniform Buffer
	//CommitGraphicResourceTables();
	//ZYZ_TODO:CommitShaderConstants Uniform Buffer
	//CommitNonComputeShaderConstants();

	uint32 SizeFormat = sizeof(DXGI_FORMAT);
	DXGI_FORMAT Format = D11IndexBuffer->GetStride() == sizeof(uint16) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;

	StateCache.SetIndexBuffer(D11IndexBuffer->Resource, Format, 0);
	StateCache.SetPrimitiveTopology(GetD3D11PrimitiveType(PrimitiveType , bUseTesslation));

	if (NumInstances > 1)
	{
		Direct3DDeviceIMContext->DrawIndexedInstancedIndirect(D11StructureBuffer->Resource, DrawArgumentIndex * 5 * sizeof(uint32));
	}
}

void FD3D11DynamicRHI::RHIDrawIndexedPrimitive(FRHIIndexBuffer* IndexBuffer, uint32 PrimitiveType, int32 BaseVertexIndex, uint32 FirstInstance, uint32 NumVertices, uint32 StartIndex, uint32 NumPrimitives, uint32 NumInstances)
{
	FD3D11IndexBuffer* D11Indexbuffer = (FD3D11IndexBuffer*)IndexBuffer;

	//ZYZ_TODO:CommitResource Uniform Buffer
	//CommitGraphicResourceTables();
	//ZYZ_TODO:CommitShaderConstants Uniform Buffer
	//CommitNonComputeShaderConstants();

	DXGI_FORMAT Format = D11Indexbuffer->GetStride() == sizeof(uint16)?DXGI_FORMAT_R16_UINT:DXGI_FORMAT_R32_UINT;

	StateCache.SetIndexBuffer(D11Indexbuffer->Resource, Format, 0);
	StateCache.SetPrimitiveTopology(GetD3D11PrimitiveType(PrimitiveType, bUseTesslation));

	uint32 IndexCount = GetVertexCountForPrimitiveCount(NumPrimitives, PrimitiveType);

	if (NumInstances > 1 || FirstInstance != 0)
	{
		Direct3DDeviceIMContext->DrawIndexedInstanced(IndexCount , NumInstances , StartIndex , BaseVertexIndex , FirstInstance);
	}
	else
	{
		Direct3DDeviceIMContext->DrawIndexed(IndexCount , StartIndex , BaseVertexIndex);
	}
}

void FD3D11DynamicRHI::RHIDrawIndexedPrimitiveIndirect(uint32 PrimitiveType, FRHIIndexBuffer* IndexBuffer, FRHIVertexBuffer* ArgumentBuffer, uint32 ArgumentOffset)
{
	FD3D11IndexBuffer* D11IndexBuffer = (FD3D11IndexBuffer*)IndexBuffer;
	FD3D11VertexBuffer* D11VertexBuffer = (FD3D11VertexBuffer*)ArgumentBuffer;

	DXGI_FORMAT Format = D11IndexBuffer->GetStride() == sizeof(uint16) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;

	StateCache.SetIndexBuffer(D11IndexBuffer->Resource, Format, 0);
	StateCache.SetPrimitiveTopology(GetD3D11PrimitiveType(PrimitiveType , bUseTesslation));

	Direct3DDeviceIMContext->DrawIndexedInstancedIndirect(D11VertexBuffer->Resource, ArgumentOffset);
}

void FD3D11DynamicRHI::RHIBeginDrawPrimitiveUP(uint32 PrimitiveType, uint32 NumPrimitives, uint32 NumVertices, uint32 VertexDataStride, void*& OutVertexData)
{
	//Cache draw call param
	PendingPrimitiveType = PrimitiveType;
	PendingNumPrimitives = NumPrimitives;
	PendingNumVertices = NumVertices;
	PendingVertexDataStride = VertexDataStride;
	//Output Buffer
	OutVertexData = DynamicVB->Lock(NumVertices * VertexDataStride);
}

void FD3D11DynamicRHI::RHIEndDrawPrimitiveUP()
{
	//Unmap
	ID3D11Buffer* Buffer = DynamicVB->Unlock();

	//ZYZ_TODO:Don't know why
	//CommitGraphicsResourceTables();
	//CommitNonComputeShaderConstants();
	//DrawCall
	StateCache.SetStreamSource(Buffer, 0, PendingVertexDataStride, 0);
	StateCache.SetPrimitiveTopology(GetD3D11PrimitiveType(PendingPrimitiveType, bUseTesslation));
	Direct3DDeviceIMContext->Draw(PendingNumVertices, 0);

	//Clear Cache Param;
	PendingPrimitiveType = 0;
	PendingNumPrimitives = 0;
	PendingNumVertices = 0;
	PendingVertexDataStride = 0;

}

void FD3D11DynamicRHI::RHIBeginDrawIndexedPrimitiveUP(uint32 PrimitiveType, uint32 NumPrimitives, uint32 NumVertices, uint32 VertexDataStride, void*& OutVertexData, uint32 MinVertexIndex, uint32 NumIndices, uint32 IndexDataStride, void*& OutIndexData)
{
	//Cache Param
	PendingNumVertices = NumVertices;
	PendingVertexDataStride = VertexDataStride;
	PendingPrimitiveType = PrimitiveType;
	PendingNumPrimitives = NumPrimitives;
	PendingMinVertexIndex = MinVertexIndex;
	PendingNumIndices = NumIndices;
	PendingIndexDataStride = IndexDataStride;

	//Map Buffer
	OutIndexData = DynamicIB->Lock(NumIndices * IndexDataStride);
	OutVertexData = DynamicVB->Lock(NumVertices * VertexDataStride);
}

void FD3D11DynamicRHI::RHIEndDrawIndexedPrimitiveUP()
{
	//Unmap data
	ID3D11Buffer* IndexBuffer = DynamicIB->Unlock();
	ID3D11Buffer* VertexBuffer = DynamicVB->Unlock();

	//ZYZ_TODO:Don't know why
	//CommitGraphicsResourceTables();
	//CommitNonComputeShaderConstants();
	//Draw
	StateCache.SetIndexBuffer(IndexBuffer, PendingIndexDataStride == sizeof(uint16) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT, 0);
	StateCache.SetStreamSource(VertexBuffer, PendingMinVertexIndex, PendingVertexDataStride, 0);
	StateCache.SetPrimitiveTopology(GetD3D11PrimitiveType(PendingPrimitiveType, bUseTesslation));

	Direct3DDeviceIMContext->DrawIndexed(PendingNumVertices, 0, PendingMinVertexIndex);

	//ClearCache
	PendingNumVertices = 0;
	PendingVertexDataStride = 0;
	PendingPrimitiveType = 0;
	PendingNumPrimitives = 0;
	PendingMinVertexIndex = 0;
	PendingNumIndices = 0;
	PendingIndexDataStride = 0;
}