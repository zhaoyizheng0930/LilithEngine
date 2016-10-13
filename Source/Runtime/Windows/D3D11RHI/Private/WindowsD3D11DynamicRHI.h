#pragma once
#include "RHI.h"
#include "D3D11RHI.h"

struct FD3D11Adapter
{
	int32 AdapterIndex;
	
	D3D_FEATURE_LEVEL MaxSupportedFeatureLevel;

	FD3D11Adapter(int32 InAdapterIndex = -1, D3D_FEATURE_LEVEL InMaxSupportedFeatureLevel = (D3D_FEATURE_LEVEL)0)
		: AdapterIndex(InAdapterIndex)
		, MaxSupportedFeatureLevel(InMaxSupportedFeatureLevel)
	{
	}

	bool IsValid() const
	{
		return MaxSupportedFeatureLevel != (D3D_FEATURE_LEVEL)0 && AdapterIndex >= 0;
	}
};

class FD3D11DynamicRHI:public FDynamicRHI
{
public:
	FD3D11DynamicRHI(IDXGIFactory1* InDXGIFactory1, D3D_FEATURE_LEVEL InFeatureLevel, int32 InChosenAdapter, const DXGI_ADAPTER_DESC& InChosenDescription);

	~FD3D11DynamicRHI();

	virtual void Init();

	virtual void PostInit();

	virtual void Shutdown();

	void InitD3DDevice();

	virtual void ClearState();
protected:
private:
	IDXGIFactory1* DXGIFactory1;
	D3D_FEATURE_LEVEL FeatureLevel;
	int32 ChosenAdapter;
	DXGI_ADAPTER_DESC ChosenDescription;

	FD3D11Device* Direct3DDevice;
};