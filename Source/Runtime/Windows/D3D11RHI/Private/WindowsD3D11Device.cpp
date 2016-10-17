#include "D3D11RHIPCH.h"
#include "WindowsD3D11Device.h"
#include "RHI.h"
#include "WindowsD3D11DynamicRHI.h"
#include "Core.h"

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