#pragma once
#include "RHI.h"
#include "D3D11RHI.h"
#include "DynamicRHI.h"

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
	virtual void RHIFlushComputeShaderCache() final override;

	virtual void RHIAutomaticCacheFlushAfterComputeShader(bool bEnable) final override;

	virtual void RHIClearUAV(FRHIUnorderedAccessView* UAV, uint32* Values) final override;

	virtual void RHICopyToResolveTarget(FRHITexture* SourceTexture, FRHITexture* DestTexture, bool bKeepOriginalSurface, const FResolveParams& ResolveParam) final override;

	virtual void RHITransitionResources(EResourceTransitionAccess TransitionType, FRHITexture** InTextures, int32 NumTextures) final override;//ZYZ_TODO:Implement later

	virtual void RHITransitionResources(EResourceTransitionAccess TransitionType, EResourceTransitionPipeline TransitionPipeline, FRHIUnorderedAccessView* InUAVs, int32 NumUAVs, FRHIComputeFence* WriteComputeFence) final override;//ZYZ_TODO:Implement later

	//Command Hint--------------------------------------------------------------------------------------------
	virtual void RHISubmitCommandsHint() final override;

	//Frame-------------------------------------------------------------------------------------------------------
	virtual void RHIBeginFram() final override;

	virtual void RHIEndFram() final override;

	//Scene------------------------------------------------------------------------------------------------
	virtual void RHIBeginScene() final override;

	virtual void RHIEndScene() final override;

	//Viewport---------------------------------------------------------------------------------------------
	virtual void RHISetMultipleViewPorts(uint32 Count, FViewportBound* Data) final override;

	virtual void RHIBeginDrawingViewport(FRHIViewport* Viewport, FRHITexture* RenderTarget) final override;

	virtual void RHIEndDrawingViewport(FRHIViewport* Viewport, bool bPresent, bool bLockVsync) final override;

	//ResourceBind---------------------------------------------------------------------------------------------Query
	virtual void RHIBeginRenderQuery(FRHIRenderQuery* RenderQuery) final override;

	virtual void RHIEndRenderQuery(FRHIRenderQuery* RenderQuery) final override;

	virtual void RHIBeginOcclusionQueryBatch() final override;

	virtual void RHIEndOcclusionQueryBatch() final override;

	//ResourceBind----------------------------------------------------------------------------------------------VertexBuffer
	virtual void RHISetStreamSource(uint32 StreamIndex, FRHIVertexBuffer* VertexBuffer, uint32 Stride, uint32 Offset) final override;

	//FourStates----------------------------------------------------------------------------------------------
	virtual void RHISetRasterizerState(FRHIRasterizerState* RasterizerState) final override;

	virtual void RHISetScissorRect(bool bEnable, uint32 MinX, uint32 MinY, uint32 MaxX, uint32 MaxY) final override;

	virtual void RHISetDepthStencilState(FRHIDepthStencilState* NewState, uint32 StencilRef) final override;

	virtual void RHISetBlendState(FRHIBlendState* NewState, const FLinearColor& BlendFactor) final override;

	//ResourceBind----------------------------------------------------------------------------------------------ViewportSet
	virtual void RHISetViewport(uint32 MinX, uint32 MinY, float MinZ, uint32 MaxX, uint32 MaxY, float MaxZ) final override;

	virtual void RHISetStereoViewport(uint32 LeftMinX, uint32 RightMinX, uint32 MinY, float MinZ, uint32 LeftMaxX, uint32 RightMaxX, uint32 MaxY, float MaxZ)  final override;//Seems like use for VR

	//ResourceBind----------------------------------------------------------------------------------------------Shader;ZYZ_TODO:SupportLater
	virtual void RHISetBoundShaderState(FRHIBoundShaderState* BoundShaderState) final override;

	//ResourceBind----------------------------------------------------------------------------------------------ShaderTexture
	virtual void RHISetShaderTexture(FRHIVertexShader* VertexShader, uint32 TextureIndex, FRHITexture* NewTexture) final override;

	virtual void RHISetShaderTexture(FRHIHullShader* HullShader, uint32 TextureIndex, FRHITexture* NewTexture) final override;

	virtual void RHISetShaderTexture(FRHIDomainShader* DomainShader, uint32 TextureIndex, FRHITexture* NewTexture) final override;

	virtual void RHISetShaderTexture(FRHIGeometryShader* GeometryShader, uint32 TextureIndex, FRHITexture* NewTexture) final override;

	virtual void RHISetShaderTexture(FRHIComputeShader* ComputeShader, uint32 TextureIndex, FRHITexture* NewTexture) final override;

	virtual void RHISetShaderTexture(FRHIPixelShader* PixelShader, uint32 TextureIndex, FRHITexture* NewTexture) final override;

	//ResourceBind----------------------------------------------------------------------------------------------ShaderSampler
	virtual void RHISetShaderSampler(FRHIVertexShader* VertexShader, uint32 SamplerIndex, FRHISamplerState* NewState) final override;

	virtual void RHISetShaderSampler(FRHIHullShader* HullShader, uint32 SamplerIndex, FRHISamplerState* NewState) final override;

	virtual void RHISetShaderSampler(FRHIDomainShader* DomainShader, uint32 SamplerIndex, FRHISamplerState* NewState) final override;

	virtual void RHISetShaderSampler(FRHIGeometryShader* GeometryShader, uint32 SamplerIndex, FRHISamplerState* NewState) final override;

	virtual void RHISetShaderSampler(FRHIComputeShader* ComputeShader, uint32 SamplerIndex, FRHISamplerState* NewState) final override;

	virtual void RHISetShaderSampler(FRHIPixelShader* PixelShader, uint32 SamplerIndex, FRHISamplerState* NewState)  final override;

	//ResourceBind----------------------------------------------------------------------------------------------UAV
	virtual void RHISetUAVParameter(FRHIComputeShader* ComputeShader, uint32 UAVIndex, FRHIUnorderedAccessView* UAV)  final override;

	virtual void RHISetUAVParameter(FRHIComputeShader* ComputeShader, uint32 UAVIndex, FRHIUnorderedAccessView* UAV, uint32 InitialCount)  final override;

	//ResourceBind----------------------------------------------------------------------------------------------SRV
	virtual void RHISetShaderResourceViewParameter(FRHIVertexShader* VertexShader, uint32 SRVIndex, FRHIShaderResourceView* SRV)  final override;

	virtual void RHISetShaderResourceViewParameter(FRHIHullShader* HullShader, uint32 SRVIndex, FRHIShaderResourceView* SRV)  final override;

	virtual void RHISetShaderResourceViewParameter(FRHIDomainShader* DomainShader, uint32 SRVIndex, FRHIShaderResourceView* SRV)  final override;

	virtual void RHISetShaderResourceViewParameter(FRHIGeometryShader* GeometryShader, uint32 SRVIndex, FRHIShaderResourceView* SRV)  final override;

	virtual void RHISetShaderResourceViewParameter(FRHIComputeShader* ComputeShader, uint32 SRVIndex, FRHIShaderResourceView* SRV)  final override;

	virtual void RHISetShaderResourceViewParameter(FRHIPixelShader* PixelShader, uint32 SRVIndex, FRHIShaderResourceView* SRV)  final override;

	//ResourceBind----------------------------------------------------------------------------------------------RTV
	virtual void RHISetRenderTarget(uint32 NumSimulataneousRenderTargets, const FRHIRenderTargetView* NewRenderTarget, const FRHIDepthRenderTargetView* NewDepthStencilTarget, uint32 NumUAVs, FRHIUnorderedAccessView** UAVs)  final override;

	virtual void RHISetRenderTargetAndClear(FRHISetRenderTargetsInfo* RenderTargetInfo)  final override;

	//ResourceBind--------ZYZ_TODO:can't used!!!!!!---no ResourceCommit--------------------------------------------------------------------------------------UniformBuffer(Shader Resource)
	virtual void RHISetShaderUniformBuffer(FRHIVertexShader* VertexShader, uint32 BufferIndex, FRHIUniformBuffer* Buffer) final override;

	virtual void RHISetShaderUniformBuffer(FRHIHullShader* HullShader, uint32 BufferIndex, FRHIUniformBuffer* Buffer) final override;

	virtual void RHISetShaderUniformBuffer(FRHIDomainShader* DomainShader, uint32 BufferIndex, FRHIUniformBuffer* Buffer) final override;

	virtual void RHISetShaderUniformBuffer(FRHIGeometryShader* GeometryShader, uint32 BufferIndex, FRHIUniformBuffer* Buffer) final override;

	virtual void RHISetShaderUniformBuffer(FRHIComputeShader* ComputeShader, uint32 BufferIndex, FRHIUniformBuffer* Buffer) final override;

	virtual void RHISetShaderUniformBuffer(FRHIPixelShader* PixelShader, uint32 BufferIndex, FRHIUniformBuffer* Buffer) final override;

	//ResourceBind----------------------------------------------------------------------------------------------Set C Buffer
	virtual void RHISetShaderParameter(FRHIVertexShader* VertexShader, uint32 BufferIndex, uint32 BaseIndex, uint32 NumBytes, const void* NewValue) final override;

	virtual void RHISetShaderParameter(FRHIHullShader* HullShader, uint32 BufferIndex, uint32 BaseIndex, uint32 NumBytes, const void* NewValue) final override;

	virtual void RHISetShaderParameter(FRHIDomainShader* DomainShader, uint32 BufferIndex, uint32 BaseIndex, uint32 NumBytes, const void* NewValue) final override;

	virtual void RHISetShaderParameter(FRHIGeometryShader* GeometryShader, uint32 BufferIndex, uint32 BaseIndex, uint32 NumBytes, const void* NewValue) final override;

	virtual void RHISetShaderParameter(FRHIComputeShader* ComputeShader, uint32 BufferIndex, uint32 BaseIndex, uint32 NumBytes, const void* NewValue) final override;

	virtual void RHISetShaderParameter(FRHIPixelShader* PixelShader, uint32 BufferIndex, uint32 BaseIndex, uint32 NumBytes, const void* NewValue) final override;

	//DrawCall----------------------------------------------------------------------------------------------------------------------------------
	virtual void RHIDrawPrimitive(uint32 PrimitiveType, uint32 BaseVertexIndex, uint32 NumPrimitives, uint32 NumInstances) final override;

	virtual void RHIDrawPrimitiveIndirect(uint32 PrimitiveType, FRHIVertexBuffer* ArgumentBuffer, uint32 ArgumentOffset)  final override;//Indirect mean Draw GPU Primitive

	virtual void RHIDrawIndexIndirect(FRHIIndexBuffer* IndexBuffer, uint32 PrimitiveType, FRHIStructureBuffer* StructureBuffer, int32 DrawArgumentIndex, uint32 NumInstances) final override;

	virtual void RHIDrawIndexedPrimitive(FRHIIndexBuffer* IndexBuffer, uint32 PrimitiveType, int32 BaseVertexIndex, uint32 FirstInstance, uint32 NumVertices, uint32 StartIndex, uint32 NumPrimitives, uint32 NumInstances)  final override;

	virtual void RHIDrawIndexedPrimitiveIndirect(uint32 PrimitiveType, FRHIIndexBuffer* IndexBuffer, FRHIVertexBuffer* ArgumentBuffer, uint32 ArgumentOffset)  final override;
	//DrawPrimitiveUp
	virtual void RHIBeginDrawPrimitiveUP(uint32 PrimitiveType, uint32 NumPrimitives, uint32 NumVertices, uint32 VertexDataStride, void*& OutVertexData) final override;

	virtual void RHIEndDrawPrimitiveUP() final override;

	virtual void RHIBeginDrawIndexedPrimitiveUP(uint32 PrimitiveType, uint32 NumPrimitives, uint32 NumVertices, uint32 VertexDataStride, void*& OutVertexData, uint32 MinVertexIndex, uint32 NumIndices, uint32 IndexDataStride, void*& OutIndexData) final override;

	virtual void RHIEndDrawIndexedPrimitiveUP() final override;

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
	//RTV Cache
	ID3D11RenderTargetView* CurrentRenderTargets[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
	//UAV Cache
	ID3D11UnorderedAccessView* CurrentUAVs[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
	//DSV Cache
	ID3D11DepthStencilView* CurrentDepthStencilTarget;
	FExclusiveDepthStencil CurrentDSVAccessType;
	FD3D11TextureBase* CurrentDepthTexture;

	//SRVDirty
	FD3D11BaseShaderResource* CurrentResourcesBoundAsSRVs[SF_NumFrequencies][D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
	int32 MaxBoundShaderResourcesIndex[SF_NumFrequencies];
	uint32 NumSimultaneousRenderTargets;
	uint32 NumUAVs;
	//DynamicBuffer
	FD3D11DynamicBuffer* DynamicVB;
	FD3D11DynamicBuffer* DynamicIB;
	//StateFor begin/end draw primitive up interface
	uint32 PendingNumVertices;
	uint32 PendingVertexDataStride;
	uint32 PendingPrimitiveType;
	uint32 PendingNumPrimitives;
	uint32 PendingMinVertexIndex;
	uint32 PendingNumIndices;
	uint32 PendingIndexDataStride;

	//CBuffer Dirty
	FD3D11UniformBuffer* BoundUniformBuffers[SF_NumFrequencies][14];
	uint16 DirtyUniformBuffers[SF_NumFrequencies];

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

	FD3D11Viewport* DrawingViewport;

	FD3D11ComputeShader* CurrentComputeShader;

	uint32 PresentCounter;

	bool bUseTesslation;

	TBoundShaderStateHistory<1000> BoundShaderStateHistory;
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
	//Context Private-----------------------------------------------------------------CommitResource
	void CommitRenderTargetsAndUAVs();

	void CommitGraphicResourceTables();//Uniform Buffer

	void CommitNonComputeShaderConstants();//Uniform Buffer

	template <class ShaderType> void SetResourcesFromTables(const ShaderType* RESTRICT);
	//Clear                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
	void ClearAllShaderResources();

	template <EShaderFrequency ShaderFrequency>
	void ClearShaderResourceViews(FD3D11BaseShaderResource* Resource); //Clear One

	template <EShaderFrequency ShaderFrequency>
	void ClearAllShaderResourcesForFrequency(); //Clear All

	template <EShaderFrequency ShaderFrequency>
	void SetShaderResourceView(FD3D11BaseShaderResource* Resource, ID3D11ShaderResourceView* SRV, int32 ResourceIndex, std::string SRVName, FD3D11StateCache::ESRV_Type SrvType = FD3D11StateCache::SRV_Unknown)
	{
		InternalSetShaderResourceView<ShaderFrequency>(Resource, SRV, ResourceIndex, SRVName, SrvType);
	}

	template <EShaderFrequency ShaderFrequency>
	void InternalSetShaderResourceView(FD3D11BaseShaderResource* Resource, ID3D11ShaderResourceView* SRV, int32 ResourceIndex, std::string SRVName, FD3D11StateCache::ESRV_Type SrvType = FD3D11StateCache::SRV_Unknown);
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