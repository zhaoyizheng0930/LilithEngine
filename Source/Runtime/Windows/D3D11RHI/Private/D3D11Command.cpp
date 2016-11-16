#include "D3D11RHIPCH.h"
#include "WindowsD3D11DynamicRHI.h"

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