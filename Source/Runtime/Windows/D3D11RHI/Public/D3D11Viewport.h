#pragma once
#include "D3D11RHI.h"
#include "D3D11Resources.h"
#include "RenderUtils.h"

static DXGI_FORMAT GetRenderTargetFormat(EPixelFormat PixelFormat)
{
	DXGI_FORMAT	DXFormat = (DXGI_FORMAT)GPixelFormats[PixelFormat].PlatformFormat;
	switch (DXFormat)
	{
	case DXGI_FORMAT_B8G8R8A8_TYPELESS:		return DXGI_FORMAT_B8G8R8A8_UNORM;
	case DXGI_FORMAT_BC1_TYPELESS:			return DXGI_FORMAT_BC1_UNORM;
	case DXGI_FORMAT_BC2_TYPELESS:			return DXGI_FORMAT_BC2_UNORM;
	case DXGI_FORMAT_BC3_TYPELESS:			return DXGI_FORMAT_BC3_UNORM;
	case DXGI_FORMAT_R16_TYPELESS:			return DXGI_FORMAT_R16_UNORM;
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:		return DXGI_FORMAT_R8G8B8A8_UNORM;
	default: 								return DXFormat;
	}
}

class FD3D11EventQuery : public FRenderResource
{
public:
	FD3D11EventQuery(FD3D11DynamicRHI* InD3DRHI)
		:D3DRHI(InD3DRHI)
	{

	}

	void IssueEvent();

	void WaitForCompletion();

	// FRenderResource interface.
	virtual void InitDynamicRHI() override;
	virtual void ReleaseDynamicRHI() override;

protected:
private:
	FD3D11DynamicRHI* D3DRHI;

	ID3D11Query* Query;
};

class FD3D11Viewport:public FRHIViewport
{
public:
	FD3D11Viewport(class FD3D11DynamicRHI* InD3DRHI, HWND InWindowHandle, uint32 InSizeX, uint32 InSizeY, bool bInIsFullscreen, EPixelFormat InPreferredPixelFormat);
	virtual ~FD3D11Viewport();

	void Resize(uint32 InSizeX, uint32 InSizeY, bool bInIsFullscreen);

	void ConditionalResetSwapChain(bool bIgnoreFocus);

	bool Present(bool bLockToVsync);

	FD3D11Texture2D* GetBackBuffer() { return BackBuffer; }

	void WaitForFrameEventCompletion()
	{
		FrameSyncEvent.WaitForCompletion();
	}

	void IssueFrameEvent()
	{
		FrameSyncEvent.IssueEvent();
	}
protected:
private:
	FD3D11Texture2D* GetSwapChainSurface(FD3D11DynamicRHI* D3DRHI, EPixelFormat PixelFormat, IDXGISwapChain* SwapChain);

	bool PresentChecked(int32 SyncInterval);
private:

	FD3D11DynamicRHI* D3DRHI;

	uint32 SizeX;

	uint32 SizeY;

	bool bIsFullScreen;

	bool bIsValid;

	HWND WindowHandle;

	EPixelFormat PixelFormat;

	IDXGISwapChain* SwapChain;

	FD3D11Texture2D* BackBuffer;

	FD3D11EventQuery FrameSyncEvent;

	//Seems like use for VR
	FRHICustomPresent* CustomPresent;
};