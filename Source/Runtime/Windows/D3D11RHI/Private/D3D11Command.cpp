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