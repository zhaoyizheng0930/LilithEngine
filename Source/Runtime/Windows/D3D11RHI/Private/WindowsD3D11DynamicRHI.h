#pragma once
#include "RHI.h"
#include "D3D11RHI.h"
#include "DynamicRHI.h"
#include "D3D11Resources.h"
#include "Windows/D3D11StateCache.h"
#include "D3D11Viewport.h"
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

class FD3D11DynamicRHI:public FDynamicRHI, public IRHICommandContext
{
public:
	FD3D11DynamicRHI(IDXGIFactory1* InDXGIFactory1, D3D_FEATURE_LEVEL InFeatureLevel, int32 InChosenAdapter, const DXGI_ADAPTER_DESC& InChosenDescription);

	~FD3D11DynamicRHI();

	virtual void Init();

	virtual void PostInit();

	virtual void Shutdown();

	virtual void InitD3DDevice();

	virtual void ClearState();
private:

	void SetupAfterDeviceCreation();

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

	virtual FRHITexture2D* RHICreateTexture2D(uint32 SizeX, uint32 SizeY, uint8 Format, uint32 NumMips, uint32 NumSamples, uint32 Flags, FRHIResourceCreateInfo& CreateInfo) final override;

	virtual FRHITexture2D* RHIAsyncCreateTexture2D(uint32 SizeX, uint32 SizeY, uint8 Format, uint32 NumMips, uint32 Flags, void** InitialMipData, uint32 NumInitialMips) final override;

	virtual void RHICopySharedMips(FRHITexture2D* DestTexture2D, FRHITexture2D* SrcTexture2D) final override;

	virtual FRHITexture2DArray* RHICreateTexture2DArray(uint32 SizeX, uint32 SizeY, uint32 SizeZ, uint8 Format, uint32 NumMips, uint32 Flags, FRHIResourceCreateInfo& CreateInfo) final override;

	virtual FRHITexture3D* RHICreateTexture3D(uint32 SizeX, uint32 SizeY, uint32 SizeZ, uint8 Format, uint32 NumMips, uint32 NumSamples, uint32 Flags, FRHIResourceCreateInfo& CreateInfo) final override;

	virtual FRHITextureCube* RHICreateTextureCube(uint32 Size, uint8 Format, uint32 NumMips, uint32 Flags, FRHIResourceCreateInfo& CreateInfo) final override;

	virtual FRHITextureCube* RHICreateTextureCubeArray(uint32 Size, uint32 ArraySize, uint8 Format, uint32 NumMips, uint32 Flags, FRHIResourceCreateInfo& CreateInfo) final override;

	virtual uint64 RHICalcTexture2DPlatformSize(uint32 SizeX, uint32 SizeY, uint8 Format, uint32 NumMips, uint32 Flag, uint32& OutAlign) final override;

	virtual uint64 RHICalcTexture3DPlatformSize(uint32 SizeX, uint32 SizeY, uint32 SizeZ, uint8 Format, uint32 NumMips, uint32 Flag, uint32& OutAlign) final override;

	virtual uint64 RHICalcTextureCubePlatformSize(uint32 Size, uint8 Format, uint32 NumMips, uint32 Flag, uint32& OutAlign) final override;

	virtual bool RHIGetTextureMemoryVisualizeData(FColor* TextureData, int32 SizeX, int32 SizeY, int32 Pitch, int32 PixelSize) final override;

	virtual void RHIGenerateMips(FRHITexture* Texture) final override;

	virtual uint32 RHIComputeMemorySize(FRHITexture* Texture) final override;

	virtual ETextureReallocationStatus RHIFinalizeAsyncReallocateTexture2D(FRHITexture2D* Texture2D, bool bBlockUntilCompleted) final override;

	virtual ETextureReallocationStatus RHICancelAsyncReallocateTexture2D(FRHITexture2D* Texture2D, bool bBlockUntilCompleted) final override;

	virtual void* RHILockTexture2D(FRHITexture2D* Texture, uint32 MipIndex, EResourceLockMode LockMode, uint32& DestStride, bool bLockWithinMiptail) final override;

	virtual void RHIUnlockTexture2D(FRHITexture2D* Texture, uint32 MipIndex, bool bLockWithinMiptail) final override;

	virtual void* RHILockTexture2DArray(FRHITexture2DArray* TextureArray, uint32 TextureIndex, uint32 MipIndex, EResourceLockMode LockMode, uint32& DestStride, bool bLockWithInMiptail) final override;

	virtual void RHIUnlockTexture2DArray(FRHITexture2DArray* TextureArray, uint32 TextureIndex, uint32 MipIndex, bool bLockWithMiptail) final override;

	virtual void* RHILockTextureCubeFace(FRHITextureCube* TextureCube, uint32 FaceIndex, uint32 ArrayIndex, uint32 MipIndex, EResourceLockMode LockMode, uint32& DestStride, bool bLockWithInMiptail) final override;

	virtual void RHIUnLockTextureCubeFace(FRHITextureCube* TextureCube, uint32 FaceIndex, uint32 ArrayIndex, uint32 MipIndex, bool bLockWithinMiptail) final override;

	virtual void RHIUpdateTexture2D(FRHITexture2D* Texture2D, uint32 MipLevel, FUpdateTextureRegion2D& Region2D, uint32 SourcePitch, uint8* SourceData) = 0;

	virtual void RHIUpdateTexture3D(FRHITexture3D* Texture3D, uint32 MipLevel, FUpdateTextureRegion3D& Region3D, uint32 SourceRowPitch, uint32 SourceDepthPitch, uint8* SourceData) = 0;

	virtual FRHIUnorderedAccessView* RHICreateUnorderedAccessView(FRHITexture* Texture, uint32 MipLevel)  final override;

	virtual FRHIUnorderedAccessView* RHICreateUnorderedAccessView(FRHIStructureBuffer* StructBuffer, bool bUseUAVCounter, bool bAppendBuffer)  final override;

	virtual FRHIUnorderedAccessView* RHICreateUnorderedAccessView(FRHIVertexBuffer* VertexBuffer, uint8 Format)  final override;

	virtual FRHIShaderResourceView* RHICreateShaderResourceView(FRHIStructureBuffer* StructBuffer)  final override;

	virtual FRHIShaderResourceView* RHICreateShaderResourceView(FRHIVertexBuffer* VertexBuffer, uint32 Stride, uint8 Format)  final override;

	virtual FRHIShaderResourceView* RHICreateShaderResourceView(FRHIIndexBuffer* IndexBuffer)  final override;

	virtual FRHIShaderResourceView* RHICreateShaderResourceView(FRHITexture2D* Texture2D, uint8 MipLevel)  final override;

	virtual FRHIShaderResourceView* RHICreateShaderResourceView(FRHITexture2D* Texture2D, uint8 MipLevel, uint8 NumMipLevels, uint8 Format)  final override;

	virtual FRHIShaderResourceView* RHICreateShaderResourceView(FRHITexture3D* Texture3D, uint8 MipLevel)  final override;

	virtual FRHIShaderResourceView* RHICreateShaderResourceView(FRHITexture2DArray* Texture2DArray, uint8 MipLevel)  final override;

	virtual FRHIShaderResourceView* RHICreateShaderResourceView(FRHITextureCube* TextureCube, uint8 MipLevel)  final override;

	virtual FRHIRenderQuery* RHICreateRenderQuery(ERenderQueryType QueryType)  final override;

	virtual bool RHIGetRenderQueryResult(FRHIRenderQuery* RenderQuery, uint64& OutResult, bool bWait)  final override;

	virtual FRHIViewport* RHICreateViewport(void* WindowsHandle, uint32 SizeX, uint32 SizeY, bool bIsFullScreen, EPixelFormat PreferredPixelFormat)  final override;

	virtual void RHIResizeViewport(FRHIViewport* Viewport, uint32 SizeX, uint32 SizeY, bool bIsFullScreen)  final override;

	virtual FRHITexture* RHIGetViewportBackBuffer(FRHIViewport* Viewport)  final override;

	virtual void RHIAdvanceFrameForGetViewportBackBuffer()  final override;

	virtual void RHIAcquireThreadOwnership() final override {};

	virtual void RHIReleaseThreadOwnership() final override {};

	virtual void RHIFlushResource() final override {};

	virtual uint32 RHIGetGPUFrameCycles() final override { return 0; };//ZYZ_TODO:GPU Time support later
	virtual void RHITick(float DeltaTime) final override;
	virtual void RHISuspendRendering() final override;
	virtual void RHIResumeRendering() final override;


	//----------------------------------------------------Context--------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------------------------
	virtual void RHIFlushComputeShaderCache();

	virtual void RHIAutomaticCacheFlushAfterComputeShader(bool bEnable);

	virtual void RHIClearUAV(FRHIUnorderedAccessView* UAV, uint32* Values);

	virtual void RHICopyToResolveTarget(FRHITexture* SourceTexture, FRHITexture* DestTexture, bool bKeepOriginalSurface, const FResolveParams& ResolveParam);

	//Viewport---------------------------------------------------------------------------------------------
	virtual void RHISetMultipleViewPorts(uint32 Count, FViewportBound* Data) = 0;

	virtual void RHIBeginDrawingViewport(FRHIViewport* Viewport, FRHITexture* RenderTargetRHI) = 0;

	virtual void RHIEndDrawingViewport(FRHIViewport* Viewport, bool bPresent, bool bLockVsync) = 0;

	//ResourceBind---------------------------------------------------------------------------------------------Query
	virtual void RHIBeginRenderQuery(FRHIRenderQuery* RenderQuery) = 0;

	virtual void RHIEndRenderQuery(FRHIRenderQuery* RenderQuery) = 0;

	virtual void RHIBeginOcclusionQueryBatch() = 0;

	virtual void RHIEndOcclusionQueryBatch() = 0;



public:
	ID3D11Device* GetDevice() { return Direct3DDevice; }

	ID3D11DeviceContext* GetContext() { return Direct3DDeviceIMContext; }

	std::map<FD3D11LockedKey, FD3D11LockedData>& GetOutstandingLocks() {
		return OutstandingLocks;
	}

	IDXGIFactory1* GetFactory() {
		return DXGIFactory1;
	}

	std::vector<FD3D11Viewport*> Viewports;
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

	void* ZeroMemory;
	uint32 ZeroMemorySize;
protected:
	/** Initializes the constant buffers.  Called once at RHI initialization time. */
	void InitConstantBuffers();

	template<typename BaseResourceType>
	TD3D11Texture2D<BaseResourceType>* CreateD3D11Texture2D(uint32 SizeX, uint32 SizeY, uint32 SizeZ, bool bTextureArray, bool CubeTexture, uint8 Format,
		uint32 NumMips, uint32 NumSamples, uint32 Flags, FRHIResourceCreateInfo& CreateInfo);

	FD3D11Texture3D* CreateD3D11Texture3D(uint32 SizeX, uint32 SizeY, uint32 SizeZ, uint8 Format, uint32 NumMips, uint32 Flags );

	uint32 GetMaxMSAAQuality(int InActualMSAACount);

	bool GetQueryData(ID3D11Query* Query, void* Data, SIZE_T DataSize, bool bWait, ERenderQueryType QueryType);

	template<typename TPixelShader>
	void ResolveTextureUsingShader(
		FRHICommandList_RecursiveHazardous& RHICmdList,
		FD3D11Texture2D* SourceTexture,
		FD3D11Texture2D* DestTexture,
		ID3D11RenderTargetView* DestSurfaceRTV,
		ID3D11DepthStencilView* DestSurfaceDSV,
		const D3D11_TEXTURE2D_DESC& ResolveTargetDesc,
		const FResolveRect& SourceRect,
		const FResolveRect& DestRect,
		FD3D11DeviceContext* Direct3DDeviceContext,
		typename TPixelShader::FParameter PixelShaderParameter
		);
private:

};


/** Find an appropriate DXGI format for the input format and SRGB setting. */
inline DXGI_FORMAT FindShaderResourceDXGIFormat(DXGI_FORMAT InFormat, bool bSRGB)
{
	if (bSRGB)
	{
		switch (InFormat)
		{
		case DXGI_FORMAT_B8G8R8A8_TYPELESS:    return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
		case DXGI_FORMAT_R8G8B8A8_TYPELESS:    return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		case DXGI_FORMAT_BC1_TYPELESS:         return DXGI_FORMAT_BC1_UNORM_SRGB;
		case DXGI_FORMAT_BC2_TYPELESS:         return DXGI_FORMAT_BC2_UNORM_SRGB;
		case DXGI_FORMAT_BC3_TYPELESS:         return DXGI_FORMAT_BC3_UNORM_SRGB;
		case DXGI_FORMAT_BC7_TYPELESS:         return DXGI_FORMAT_BC7_UNORM_SRGB;
		};
	}
	else
	{
		switch (InFormat)
		{
		case DXGI_FORMAT_B8G8R8A8_TYPELESS: return DXGI_FORMAT_B8G8R8A8_UNORM;
		case DXGI_FORMAT_R8G8B8A8_TYPELESS: return DXGI_FORMAT_R8G8B8A8_UNORM;
		case DXGI_FORMAT_BC1_TYPELESS:      return DXGI_FORMAT_BC1_UNORM;
		case DXGI_FORMAT_BC2_TYPELESS:      return DXGI_FORMAT_BC2_UNORM;
		case DXGI_FORMAT_BC3_TYPELESS:      return DXGI_FORMAT_BC3_UNORM;
		case DXGI_FORMAT_BC7_TYPELESS:      return DXGI_FORMAT_BC7_UNORM;
		};
	}
	switch (InFormat)
	{
	case DXGI_FORMAT_R24G8_TYPELESS: return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	case DXGI_FORMAT_R32_TYPELESS: return DXGI_FORMAT_R32_FLOAT;
	case DXGI_FORMAT_R16_TYPELESS: return DXGI_FORMAT_R16_UNORM;
#if DEPTH_32_BIT_CONVERSION
		// Changing Depth Buffers to 32 bit on Dingo as D24S8 is actually implemented as a 32 bit buffer in the hardware
	case DXGI_FORMAT_R32G8X24_TYPELESS: return DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
#endif
	}
	return InFormat;
}


/** Find the appropriate depth-stencil targetable DXGI format for the given format. */
inline DXGI_FORMAT FindDepthStencilDXGIFormat(DXGI_FORMAT InFormat)
{
	switch (InFormat)
	{
	case DXGI_FORMAT_R24G8_TYPELESS:
		return DXGI_FORMAT_D24_UNORM_S8_UINT;
#if DEPTH_32_BIT_CONVERSION
		// Changing Depth Buffers to 32 bit on Dingo as D24S8 is actually implemented as a 32 bit buffer in the hardware
	case DXGI_FORMAT_R32G8X24_TYPELESS:
		return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
#endif
	case DXGI_FORMAT_R32_TYPELESS:
		return DXGI_FORMAT_D32_FLOAT;
	case DXGI_FORMAT_R16_TYPELESS:
		return DXGI_FORMAT_D16_UNORM;
	};
	return InFormat;
}

/** Find an appropriate DXGI format unordered access of the raw format. */
inline DXGI_FORMAT FindUnorderedAccessDXGIFormat(DXGI_FORMAT InFormat)
{
	switch (InFormat)
	{
	case DXGI_FORMAT_B8G8R8A8_TYPELESS: return DXGI_FORMAT_B8G8R8A8_UNORM;
	case DXGI_FORMAT_R8G8B8A8_TYPELESS: return DXGI_FORMAT_R8G8B8A8_UNORM;
	}
	return InFormat;
}