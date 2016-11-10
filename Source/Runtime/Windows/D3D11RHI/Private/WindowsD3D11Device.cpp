#include "D3D11RHIPCH.h"
#include "WindowsD3D11Device.h"
#include "RHI.h"
#include "WindowsD3D11DynamicRHI.h"
#include "RenderUtils.h"

void SafeCreateDXGIFactory(IDXGIFactory1** DXGIFactory1)
{
	CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)DXGIFactory1);
}

bool SafeTestCreateD3D11Device(IDXGIAdapter* Adapter,D3D_FEATURE_LEVEL MaxFeatureLevel,D3D_FEATURE_LEVEL* OutFeatureLevel)
{
	UINT flag = D3D11_CREATE_DEVICE_SINGLETHREADED;
	flag |= D3D11_CREATE_DEVICE_DEBUG;

	D3D_FEATURE_LEVEL featurelevel[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0
	};

	int32 FirstAllowedFeatureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Device* pDevice = nullptr;
	ID3D11DeviceContext* pImmediateContext = nullptr;

	if (D3D11CreateDevice(Adapter , D3D_DRIVER_TYPE_UNKNOWN , NULL , flag ,  featurelevel , FirstAllowedFeatureLevel , D3D11_SDK_VERSION , &pDevice
		, OutFeatureLevel, &pImmediateContext))
	{
		pDevice->Release();
		pImmediateContext->Release();
		return true;
	}
	return false;
}

//Device check
void FindAdaptor(FD3D11Adapter& ChosenAdaptor , DXGI_ADAPTER_DESC& ChosenDescription)
{
	IDXGIFactory1* DXGIFactory1;
	SafeCreateDXGIFactory(&DXGIFactory1);
	if (DXGIFactory1)
	{
		std::vector<DXGI_ADAPTER_DESC> AdapterDescs;
		D3D_FEATURE_LEVEL MaxAllowedFeatureLevel = D3D_FEATURE_LEVEL_11_0;
		IDXGIAdapter* TempAdapter = nullptr;
		for (int i = 0; DXGIFactory1->EnumAdapters(i, &TempAdapter) != DXGI_ERROR_NOT_FOUND;i++)
		{
			DXGI_ADAPTER_DESC AdapterDesc;
			ZeroMemory(&AdapterDesc, sizeof(DXGI_ADAPTER_DESC));
			//Check if adaptor support d3d11
			if (TempAdapter)
			{
				D3D_FEATURE_LEVEL ActualFeatureLevel = (D3D_FEATURE_LEVEL)0;
				if (SafeTestCreateD3D11Device(TempAdapter , MaxAllowedFeatureLevel , &ActualFeatureLevel))
				{
					if (!FAILED(TempAdapter->GetDesc(&AdapterDesc)))
					{
						AdapterDescs.push_back(AdapterDesc);
						if (!ChosenAdaptor->IsValid())
						{
							FD3D11Adapter CurrentAdapter(i, ActualFeatureLevel);
							ChosenAdaptor = CurrentAdapter;
							ChosenDescription = AdapterDesc;
						}
					}
				}
			}
		}
	}
}

FDynamicRHI* CreateRHI()
{
	FD3D11Adapter ChosenAdaptor;
	DXGI_ADAPTER_DESC ChosenDescription;

	FDynamicRHI* DynamicRHI = nullptr;
	if (!ChosenAdaptor.IsValid())
	{
		FindAdaptor(ChosenAdaptor , ChosenDescription);
	}

	IDXGIFactory1* DXGIFactory1;
	SafeCreateDXGIFactory(&DXGIFactory1);
	DynamicRHI = new FD3D11DynamicRHI(DXGIFactory1 , ChosenAdaptor.MaxSupportedFeatureLevel , ChosenAdaptor.AdapterIndex , ChosenDescription);
	return DynamicRHI;
}

FD3D11DynamicRHI::FD3D11DynamicRHI(IDXGIFactory1* InDXGIFactory1, D3D_FEATURE_LEVEL InFeatureLevel, int32 InChosenAdapter, const DXGI_ADAPTER_DESC& InChosenDescription):
	Direct3DDevice(nullptr),
	DXGIFactory1(InDXGIFactory1),
	FeatureLevel(InFeatureLevel),
	ChosenAdapter(InChosenAdapter),
	ChosenDescription(InChosenDescription)
{
	GTexturePoolSize = 0;

	//Init PixelFormat
	GPixelFormats[PF_Unknown].PlatformFormat = DXGI_FORMAT_UNKNOWN;
	GPixelFormats[PF_A32B32G32R32F].PlatformFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
	GPixelFormats[PF_B8G8R8A8].PlatformFormat = DXGI_FORMAT_B8G8R8A8_TYPELESS;
	GPixelFormats[PF_G8].PlatformFormat = DXGI_FORMAT_R8_UNORM;
	GPixelFormats[PF_G16].PlatformFormat = DXGI_FORMAT_R16_UNORM;
	GPixelFormats[PF_DXT1].PlatformFormat = DXGI_FORMAT_BC1_TYPELESS;
	GPixelFormats[PF_DXT3].PlatformFormat = DXGI_FORMAT_BC2_TYPELESS;
	GPixelFormats[PF_DXT5].PlatformFormat = DXGI_FORMAT_BC3_TYPELESS;
	GPixelFormats[PF_BC4].PlatformFormat = DXGI_FORMAT_BC4_UNORM;
	GPixelFormats[PF_UYVY].PlatformFormat = DXGI_FORMAT_UNKNOWN;		// TODO: Not supported in D3D11
#if DEPTH_32_BIT_CONVERSION
	GPixelFormats[PF_DepthStencil].PlatformFormat = DXGI_FORMAT_R32G8X24_TYPELESS;
	GPixelFormats[PF_DepthStencil].BlockBytes = 5;
	GPixelFormats[PF_X24_G8].PlatformFormat = DXGI_FORMAT_X32_TYPELESS_G8X24_UINT;
	GPixelFormats[PF_X24_G8].BlockBytes = 5;
#else
	GPixelFormats[PF_DepthStencil].PlatformFormat = DXGI_FORMAT_R24G8_TYPELESS;
	GPixelFormats[PF_DepthStencil].BlockBytes = 4;
	GPixelFormats[PF_X24_G8].PlatformFormat = DXGI_FORMAT_X24_TYPELESS_G8_UINT;
	GPixelFormats[PF_X24_G8].BlockBytes = 4;
#endif
	GPixelFormats[PF_ShadowDepth].PlatformFormat = DXGI_FORMAT_R16_TYPELESS;
	GPixelFormats[PF_ShadowDepth].BlockBytes = 2;
	GPixelFormats[PF_R32_FLOAT].PlatformFormat = DXGI_FORMAT_R32_FLOAT;
	GPixelFormats[PF_G16R16].PlatformFormat = DXGI_FORMAT_R16G16_UNORM;
	GPixelFormats[PF_G16R16F].PlatformFormat = DXGI_FORMAT_R16G16_FLOAT;
	GPixelFormats[PF_G16R16F_FILTER].PlatformFormat = DXGI_FORMAT_R16G16_FLOAT;
	GPixelFormats[PF_G32R32F].PlatformFormat = DXGI_FORMAT_R32G32_FLOAT;
	GPixelFormats[PF_A2B10G10R10].PlatformFormat = DXGI_FORMAT_R10G10B10A2_UNORM;
	GPixelFormats[PF_A16B16G16R16].PlatformFormat = DXGI_FORMAT_R16G16B16A16_UNORM;
	GPixelFormats[PF_D24].PlatformFormat = DXGI_FORMAT_R24G8_TYPELESS;
	GPixelFormats[PF_R16F].PlatformFormat = DXGI_FORMAT_R16_FLOAT;
	GPixelFormats[PF_R16F_FILTER].PlatformFormat = DXGI_FORMAT_R16_FLOAT;

	GPixelFormats[PF_FloatRGB].PlatformFormat = DXGI_FORMAT_R11G11B10_FLOAT;
	GPixelFormats[PF_FloatRGB].BlockBytes = 4;
	GPixelFormats[PF_FloatRGBA].PlatformFormat = DXGI_FORMAT_R16G16B16A16_FLOAT;
	GPixelFormats[PF_FloatRGBA].BlockBytes = 8;

	GPixelFormats[PF_FloatR11G11B10].PlatformFormat = DXGI_FORMAT_R11G11B10_FLOAT;
	GPixelFormats[PF_FloatR11G11B10].BlockBytes = 4;

	GPixelFormats[PF_V8U8].PlatformFormat = DXGI_FORMAT_R8G8_SNORM;
	GPixelFormats[PF_BC5].PlatformFormat = DXGI_FORMAT_BC5_UNORM;
	GPixelFormats[PF_A1].PlatformFormat = DXGI_FORMAT_R1_UNORM; // Not supported for rendering.
	GPixelFormats[PF_A8].PlatformFormat = DXGI_FORMAT_A8_UNORM;
	GPixelFormats[PF_R32_UINT].PlatformFormat = DXGI_FORMAT_R32_UINT;
	GPixelFormats[PF_R32_SINT].PlatformFormat = DXGI_FORMAT_R32_SINT;

	GPixelFormats[PF_R16_UINT].PlatformFormat = DXGI_FORMAT_R16_UINT;
	GPixelFormats[PF_R16_SINT].PlatformFormat = DXGI_FORMAT_R16_SINT;
	GPixelFormats[PF_R16G16B16A16_UINT].PlatformFormat = DXGI_FORMAT_R16G16B16A16_UINT;
	GPixelFormats[PF_R16G16B16A16_SINT].PlatformFormat = DXGI_FORMAT_R16G16B16A16_SINT;

	GPixelFormats[PF_R5G6B5_UNORM].PlatformFormat = DXGI_FORMAT_B5G6R5_UNORM;
	GPixelFormats[PF_R8G8B8A8].PlatformFormat = DXGI_FORMAT_R8G8B8A8_TYPELESS;
	GPixelFormats[PF_R8G8].PlatformFormat = DXGI_FORMAT_R8G8_UNORM;
	GPixelFormats[PF_R32G32B32A32_UINT].PlatformFormat = DXGI_FORMAT_R32G32B32A32_UINT;
	GPixelFormats[PF_R16G16_UINT].PlatformFormat = DXGI_FORMAT_R16G16_UINT;

	GPixelFormats[PF_BC6H].PlatformFormat = DXGI_FORMAT_BC6H_UF16;
	GPixelFormats[PF_BC7].PlatformFormat = DXGI_FORMAT_BC7_TYPELESS;

	ZeroMemorySize = 1* (1 << 20);
	ZeroMemory = FMemory::Malloc(ZeroMemorySize);
	FMemory::MemZero(ZeroMemory);

}

FD3D11DynamicRHI::~FD3D11DynamicRHI()
{

}

void FD3D11DynamicRHI::Init()
{
	InitD3DDevice();
}

void FD3D11DynamicRHI::PostInit()
{

}

void FD3D11DynamicRHI::Shutdown()
{

}

void FD3D11DynamicRHI::ClearState()
{

}

void FD3D11DynamicRHI::UpdateMSAASettings()
{

}

void FD3D11DynamicRHI::InitD3DDevice()
{
	if (!Direct3DDevice)
	{
		ClearState();

		IDXGIAdapter* Adapter = nullptr;

		D3D_DRIVER_TYPE DriverType = D3D_DRIVER_TYPE_UNKNOWN;

		uint32 DeviceFlags = GUseMultithreadDevice ? 0 : D3D11_CREATE_DEVICE_SINGLETHREADED;

		if (GUseD3DDebug)
		{
			DeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
		}

		IDXGIAdapter* EnumAdapter = nullptr;
		if (DXGIFactory1->EnumAdapters(ChosenAdapter , &EnumAdapter) != DXGI_ERROR_NOT_FOUND)
		{
			if (EnumAdapter)
			{
				DXGI_ADAPTER_DESC AdapterDesc = ChosenDescription;
				Adapter = EnumAdapter;

				GRHIAdapterName = std::wstring(AdapterDesc.Description);
				GRHIVendorId = AdapterDesc.VendorId;
				GRHIDeviceId = AdapterDesc.DeviceId;

				{
					//Get DriverVersion
				}

				D3D_FEATURE_LEVEL ActualFeatureLevel = (D3D_FEATURE_LEVEL)0;
				D3D11CreateDevice( Adapter ,
					DriverType ,
					NULL,DeviceFlags ,
					&FeatureLevel ,
					1 ,
					D3D11_SDK_VERSION ,
					&Direct3DDevice ,
					&ActualFeatureLevel ,
					&Direct3DDeviceIMContext);

				StateCache.Init(Direct3DDeviceIMContext);

				SetupAfterDeviceCreation();

				// Notify all initialized FRenderResources that there's a valid RHI device to create their RHI resources for now.
				// Dynamic resources can have dependencies on static resources (with uniform buffers) and must initialized last!
			}
		}
	}
}

void FD3D11DynamicRHI::SetupAfterDeviceCreation()
{
	RHISetScissorRect(false, 0, 0, 0, 0);

	UpdateMSAASettings();
}

void FD3D11DynamicRHI::InitConstantBuffers()
{

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

void FD3D11DynamicRHI::RHIAutomaticCacheFlushAfterComputeShader(bool bEnable)
{
	// Nothing to do
}

void FD3D11DynamicRHI::RHIFlushComputeShaderCache()
{
	// Nothing to do
}