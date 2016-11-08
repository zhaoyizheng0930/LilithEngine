#pragma once
#include "D3D11RHI.h"
#include "D3D11Resources.h"

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
protected:
private:
	FD3D11Texture2D* GetSwapChainSurface(FD3D11DynamicRHI* D3DRHI, EPixelFormat PixelFormat, IDXGISwapChain* SwapChain);
private:

	FD3D11DynamicRHI* D3DRHI;

	uint32 SizeX;

	uint32 SizeY;

	bool bIsFullScreen;

	HWND WindowHandle;

	EPixelFormat PixelFormat;

	IDXGISwapChain* SwapChain;

	FD3D11Texture2D* BackBuffer;

	FD3D11EventQuery FrameSyncEvent;

	FRHICustomPresent* CustomPresent;
};