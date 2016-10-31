#pragma once
#include "RHI.h"
#include "D3D11RHI.h"
#include "DynamicRHI.h"
#include "D3D11Resources.h"
#include "Windows/D3D11StateCache.h"
#include "D3D11Util.h"

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
private:

	void InitD3DDevice();

	void SetupAfterDeviceCreation();

	virtual void ClearState();

	void UpdateMSAASettings();

public:
	virtual void RHISetScissorRect(bool bEnable, uint32 MinX, uint32 MinY, uint32 MaxX, uint32 MaxY) final override;

	virtual FRHISamplerState* RHICreateSamplerState(const FSamplerStateInitializerRHI& Initializer) final override;

	virtual FRHIRasterizerState* RHICreateRasterizerState(const FRasterizerStateInitializerRHI& Initializer) final override;

	virtual FRHIDepthStencilState* RHICreateDepthStencilState(const FDepthStencilStateInitializerRHI& Initializer) final override;

	virtual FRHIBlendState* RHICreateBlendState(const FBlendStateInitializerRHI& Initializer) final override;

	virtual FRHIVertexDeclaration* RHICreateVertexDeclaration(const FVertexDeclarationElementList& Elements) final override;

	virtual FRHIVertexShader* RHICreateVertexShader(const std::vector<uint8>& Code) final override;

	virtual FRHIHullShader* RHICreateHullShader(const std::vector<uint8>& Code) final override;

	virtual FRHIDomainShader* RHICreateDomainShader(const std::vector<uint8>& Code) final override;

	virtual FRHIGeometryShader* RHICreateGeometryShader(const std::vector<uint8>& Code) final override;

	virtual FRHIComputeShader* RHICreateComputeShader(const std::vector<uint8>& Code) final override;

	virtual FRHIPixelShader* RHICreatePixelShader(const std::vector<uint8>& Code) final override;

	virtual FRHIBoundShaderState* RHICreateBoundShaderState(FRHIVertexDeclaration* InVertexDeclaration, FRHIVertexShader* InVertexShader, FRHIHullShader* InHullShader, FRHIDomainShader* InDomainShader, FRHIGeometryShader* InGeometryShader, FRHIPixelShader* InPixelShader) final override;

	virtual FRHIUniformBuffer* RHICreateUniformBuffer(const void* Contents, const FRHIUniformBufferLayout& Layout, EUniformBufferUsage Usage) final override;

	virtual FRHIIndexBuffer* RHICreateIndexBuffer(uint32 Stride, uint32 Size, uint32 InUsage, FRHIResourceCreateInfo& CreateInfo) final override;

	virtual void* RHILockIndexBuffer(FRHIIndexBuffer* IndexBuffer, uint32 Offset, uint32 Size, EResourceLockMode LockMode)  final override;

	virtual void RHIUnlockIndexBuffer(FRHIIndexBuffer* IndexBuffer)  final override;

	virtual FRHIVertexBuffer* RHICreateVertexBuffer(uint32 Size, uint32 InUsage, FRHIResourceCreateInfo& CreateInfo)  final override;

	virtual void* RHILockVertexBuffer(FRHIVertexBuffer* VertexBuffer, uint32 Offset, uint32 Size, EResourceLockMode LockMode)  final override;

	virtual void RHIUnlockVertexBuffer(FRHIVertexBuffer* VertexBuffer)  final override;

	virtual void RHICopyVertexBuffer(FRHIVertexBuffer* SourceBufferRHI, FRHIVertexBuffer* DestBufferRHI)  final override;

	virtual FRHIStructureBuffer* RHICreateStructureBuffer(uint32 Stride, uint32 Size, uint32 InUsage, FRHIResourceCreateInfo& CreateInfo)  final override;

	virtual void* RHILockStructureBuffer(FRHIStructureBuffer* StructureBuffer, uint32 Offset, uint32 Size, EResourceLockMode LockMode)  final override;

	virtual void RHIUnlockStructureBuffer(FRHIStructureBuffer* StructureBuffer)  final override;

protected:
	IDXGIFactory1* DXGIFactory1;
	D3D_FEATURE_LEVEL FeatureLevel;
	int32 ChosenAdapter;
	DXGI_ADAPTER_DESC ChosenDescription;

	FD3D11Device* Direct3DDevice;
	FD3D11DeviceContext* Direct3DDeviceIMContext;

	FD3D11StateCache StateCache;

	ID3D11RenderTargetView* CurrentRenderTargets[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
	ID3D11UnorderedAccessView* CurrentUAVs[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
	ID3D11DepthStencilView* CurrentDepthStencilTarget;
	FD3D11TextureBase* CurrentDepthTexture;

	FD3D11BaseShaderResource* CurrentResourcesBoundAsSRVs[SF_NumFrequencies][D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];

	//TRefCountPtr<FD3D11DynamicBuffer> DynamicVB;
	//TRefCountPtr<FD3D11DynamicBuffer> DynamicIB;

	/** Global D3D11 lock list */
	std::map<FD3D11LockedKey, FD3D11LockedData> OutstandingLocks;

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