#pragma once
#include "RHI.h"
#include "D3D11RHI.h"
#include "D3D11Resources.h"
#include "Windows/D3D11StateCache.h"

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

	void SetupAfterDeviceCreation();

	virtual void ClearState();

	void UpdateMSAASettings();

public:
	virtual void RHISetScissorRect(bool bEnable, uint32 MinX, uint32 MinY, uint32 MaxX, uint32 MaxY) final override;
	//virtual class IRHICommandContext* RHIGetDefaultContext() final override;
protected:
	IDXGIFactory1* DXGIFactory1;
	D3D_FEATURE_LEVEL FeatureLevel;
	int32 ChosenAdapter;
	DXGI_ADAPTER_DESC ChosenDescription;

	FD3D11Device* Direct3DDevice;
	FD3D11DeviceContext* Direct3DDeviceIMContext;

	FD3D11StateCache StateCache;

	ID3D11RenderTargetView* CurrentRenderTargets[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
	ID3D11UnorderedAccessView* CurrentRenderTargets[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
	ID3D11DepthStencilView* CurrentDepthStencilTarget;
	FD3D11TextureBase* CurrentDepthTexture;

	FD3D11BaseShaderResource* CurrentResourcesBoundAsSRVs[SF_NumFrequencies][D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];

	//TRefCountPtr<FD3D11DynamicBuffer> DynamicVB;
	//TRefCountPtr<FD3D11DynamicBuffer> DynamicIB;

	std::vector<FD3D11ConstantBuffer*> VSConstantBuffers;
	std::vector<FD3D11ConstantBuffer*> HSConstantBuffers;
	std::vector<FD3D11ConstantBuffer*> DSConstantBuffers;
	std::vector<FD3D11ConstantBuffer*> PSConstantBuffers;
	std::vector<FD3D11ConstantBuffer*> GSConstantBuffers;
	std::vector<FD3D11ConstantBuffer*> CSConstantBuffers;

protected:
	/** Initializes the constant buffers.  Called once at RHI initialization time. */
	void InitConstantBuffers();
private:

};