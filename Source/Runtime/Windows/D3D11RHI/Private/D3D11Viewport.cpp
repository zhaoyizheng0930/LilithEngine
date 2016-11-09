#include "D3D11RHIPCH.h"
#include "WindowsD3D11DynamicRHI.h"
#include "D3D11Viewport.h"
#include "RenderResource.h"

FRHIViewport* FD3D11DynamicRHI::RHICreateViewport(void* WindowsHandle, uint32 SizeX, uint32 SizeY, bool bIsFullScreen, EPixelFormat PreferredPixelFormat)
{
	if (PreferredPixelFormat == EPixelFormat::PF_Unknown)
	{
		PreferredPixelFormat = EPixelFormat::PF_A2B10G10R10;
	}

	return new FD3D11Viewport(this , (HWND)WindowsHandle , SizeX , SizeY , bIsFullScreen , PreferredPixelFormat);
}

void FD3D11DynamicRHI::RHIResizeViewport(FRHIViewport* Viewport, uint32 SizeX, uint32 SizeY, bool bIsFullScreen)
{
	FD3D11Viewport* D11Viewport = (FD3D11Viewport*)Viewport;

	D11Viewport->Resize(SizeX, SizeY, bIsFullScreen);
}

FRHITexture* FD3D11DynamicRHI::RHIGetViewportBackBuffer(FRHIViewport* Viewport)
{
	FD3D11Viewport* D11Viewport = (FD3D11Viewport*)Viewport;

	return D11Viewport->GetBackBuffer();
}

void FD3D11DynamicRHI::RHIAdvanceFrameForGetViewportBackBuffer()
{

}




//===========================================================================================================================
FD3D11Viewport::FD3D11Viewport(class FD3D11DynamicRHI* InD3DRHI, HWND InWindowHandle, uint32 InSizeX, uint32 InSizeY, bool bInIsFullscreen, EPixelFormat InPreferredPixelFormat)
	:D3DRHI(InD3DRHI),
	SizeX(InSizeX),
	SizeY(InSizeY),
	bIsFullScreen(bInIsFullscreen),
	PixelFormat(InPreferredPixelFormat),
	WindowHandle(InWindowHandle),
	bIsValid(true),
	FrameSyncEvent(InD3DRHI)
{
	D3DRHI->Viewports.push_back(this);

	D3DRHI->InitD3DDevice();

	IDXGIDevice* DXGIDevice = nullptr;
	D3DRHI->GetDevice()->QueryInterface(IID_IDXGIDevice , (void**)(&DXGIDevice));
	//Create SwapChain
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;

	SwapChainDesc.BufferDesc.Width = SizeX;
	SwapChainDesc.BufferDesc.Height = SizeY;
	SwapChainDesc.BufferDesc.Format = GetRenderTargetFormat(PixelFormat);
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	SwapChainDesc.BufferCount = 1;
	SwapChainDesc.OutputWindow = InWindowHandle;

	// MSAA Sample count
	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.SampleDesc.Quality = 0;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;

	SwapChainDesc.Windowed = !bIsFullScreen;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	D3DRHI->GetFactory()->CreateSwapChain(DXGIDevice, &SwapChainDesc, &SwapChain);

	D3DRHI->GetFactory()->MakeWindowAssociation(WindowHandle, DXGI_MWA_NO_WINDOW_CHANGES);

	BackBuffer = GetSwapChainSurface(D3DRHI , PixelFormat , SwapChain);

	//PostMessage
	::PostMessage(WindowHandle, WM_PAINT, 0, 0);

	BeginInitResource(&FrameSyncEvent);
}


FD3D11Viewport::~FD3D11Viewport()
{
	SwapChain->SetFullscreenState(false, NULL);

	FrameSyncEvent.ReleaseResource();

	D3DRHI->Viewports.erase(std::find(D3DRHI->Viewports.begin(), D3DRHI->Viewports.end(), this));
}

void FD3D11Viewport::Resize(uint32 InSizeX, uint32 InSizeY, bool bInIsFullscreen)
{
	//release dangling
	//D3DRHI->RHISetRenderTargets();

	D3DRHI->ClearState();

	D3DRHI->GetContext()->Flush();

	if (CustomPresent != NULL)
	{
		CustomPresent->OnBackBufferResize();
	}

	delete BackBuffer;
	BackBuffer = NULL;

	if (SizeX != InSizeX || SizeY != InSizeY)
	{
		SizeX = InSizeX;
		SizeY = InSizeY;

		SwapChain->ResizeBuffers(1, SizeX, SizeY, GetRenderTargetFormat(PixelFormat), DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

		if (bInIsFullscreen)
		{
			DXGI_MODE_DESC ModeDesc;
			ModeDesc.Width = SizeX;
			ModeDesc.Height = SizeY;
			ModeDesc.Format = GetRenderTargetFormat(PixelFormat);
			ModeDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			ModeDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			SwapChain->ResizeTarget(&ModeDesc);
		}
	}

	//ZYZ_TODO:Don't know why
	//if (bIsFullScreen != bInIsFullscreen)
	//{
	//	bIsFullScreen = bInIsFullscreen;
	//	bIsValid = false;

	//	ConditionalResetSwapChain(true);
	//}

	BackBuffer = GetSwapChainSurface(D3DRHI, PixelFormat, SwapChain);
}

void FD3D11Viewport::ConditionalResetSwapChain(bool bIgnoreFocus)
{
	//ZYZ_TODO:Don't know why
}

bool FD3D11Viewport::Present(bool bLockToVsync)
{
	bool bNativelyPresented = true;
	bNativelyPresented = PresentChecked(bLockToVsync ? true : 0);
	return bNativelyPresented;
}

FD3D11Texture2D* FD3D11Viewport::GetSwapChainSurface(FD3D11DynamicRHI* D3DRHI, EPixelFormat PixelFormat, IDXGISwapChain* SwapChain)
{
	ID3D11Texture2D* BackBufferTexture = nullptr;
	SwapChain->GetBuffer(0, IID_ID3D10Texture2D, (void**)&BackBufferTexture);
	//CreateSRV
	ID3D11ShaderResourceView* SRV;
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	SRVDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	SRVDesc.Texture2D.MipLevels = 1;
	SRVDesc.Texture2D.MostDetailedMip = 0;
	D3DRHI->GetDevice()->CreateShaderResourceView(BackBufferTexture, &SRVDesc, &SRV);
	//CreaterRTVs
	std::vector<ID3D11RenderTargetView*> RTVs;
	ID3D11RenderTargetView* RTV = NULL;
	D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
	RTVDesc.Format = DXGI_FORMAT_UNKNOWN;
	RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	RTVDesc.Texture2D.MipSlice = 0;
	D3DRHI->GetDevice()->CreateRenderTargetView(BackBufferTexture, &RTVDesc, &RTV);
	RTVs.push_back(RTV);

	D3D11_TEXTURE2D_DESC BackBufferTextureDesc;
	BackBufferTexture->GetDesc(&BackBufferTextureDesc);

	//CreaterDSVs
	std::vector<ID3D11DepthStencilView*> DSVs;

	return new FD3D11Texture2D(D3DRHI , BackBufferTexture , SRV , 1 ,false , RTVs , DSVs , BackBufferTextureDesc.Width , BackBufferTextureDesc.Height , 1 , 1 , 1 , PixelFormat ,0 );
}

bool FD3D11Viewport::PresentChecked(int32 SyncInterval)
{
	SwapChain->Present(SyncInterval, 0);
	return 1;
}