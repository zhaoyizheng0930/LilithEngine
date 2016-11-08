#include "D3D11RHIPCH.h"
#include "WindowsD3D11DynamicRHI.h"
#include "D3D11Viewport.h"
#include "RenderResource.h"

FRHIViewport* FD3D11DynamicRHI::RHICreateViewport(void* WindowsHandle, uint32 SizeX, uint32 SizeY, bool bIsFullScreen, EPixelFormat PreferredPixelFormat)
{

}

void FD3D11DynamicRHI::RHIResizeViewport(FRHIViewport* Viewport, uint32 SizeX, uint32 SizeY, bool bIsFullScreen)
{

}

FRHITexture* FD3D11DynamicRHI::RHIGetViewportBackBuffer(FRHIViewport* Viewport)
{

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
	WindowHandle(InWindowHandle)
{
	D3DRHI->Viewports.push_back(this);

	D3DRHI->InitD3DDevice();

	IDXGIDevice* DXGIDevice = nullptr;
	D3DRHI->GetDevice()->QueryInterface(IID_IDXGIDevice , (void**)(&DXGIDevice));
	//Create SwapChain
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;

	SwapChainDesc.BufferDesc.Width = 

	SwapChainDesc.BufferCount = 1;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	SwapChainDesc.OutputWindow = InWindowHandle;
	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.SampleDesc.Quality = 0;
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

}

void FD3D11Viewport::Resize(uint32 InSizeX, uint32 InSizeY, bool bInIsFullscreen)
{

}

void FD3D11Viewport::ConditionalResetSwapChain(bool bIgnoreFocus)
{

}

bool FD3D11Viewport::Present(bool bLockToVsync)
{

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



	D3D11_TEXTURE2D_DESC BackBufferTextureDesc;
	BackBufferTexture->GetDesc(&BackBufferTextureDesc);

	//CreaterDSVs
	std::vector<ID3D11DepthStencilView*> DSVs;

	return new FD3D11Texture2D(D3DRHI , BackBufferTexture , SRV , 1 ,false , RTVs , DSVs , BackBufferTextureDesc.Width , BackBufferTextureDesc.Height , 1 , 1 , 1 , PixelFormat ,0 );
}