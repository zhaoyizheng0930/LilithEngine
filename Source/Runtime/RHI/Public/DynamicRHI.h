#pragma once
#include "RHIResource.h"
#include "RHI.h"
#include "Math/LilithMath.h"
#include "RHICommandList.h"
#include "RHICommandContext.h"


class FDynamicRHI
{
public:
	virtual ~FDynamicRHI() {}

	virtual void Init() = 0;

	virtual void PostInit() {}

	virtual void Shutdown() = 0;

	virtual void InitD3DDevice() = 0;

public:
	//Resource-----------------------------------------------------State
	virtual FRHISamplerState* RHICreateSamplerState(const FSamplerStateInitializerRHI& Initializer) = 0;

	virtual FRHIRasterizerState* RHICreateRasterizerState(const FRasterizerStateInitializerRHI& Initializer) = 0;

	virtual FRHIDepthStencilState* RHICreateDepthStencilState(const FDepthStencilStateInitializerRHI& Initializer) = 0;

	virtual FRHIBlendState* RHICreateBlendState(const FBlendStateInitializerRHI& Initializer) = 0;

	//Resource-----------------------------------------------------VertexDecl
	virtual FRHIVertexDeclaration* RHICreateVertexDeclaration(const FVertexDeclarationElementList& Elements) = 0;

	//Resource-----------------------------------------------------Shader
	virtual FRHIVertexShader* RHICreateVertexShader(const std::vector<uint8>& Code) = 0;

	virtual FRHIHullShader* RHICreateHullShader(const std::vector<uint8>& Code) = 0;

	virtual FRHIDomainShader* RHICreateDomainShader(const std::vector<uint8>& Code) = 0;

	virtual FRHIGeometryShader* RHICreateGeometryShader(const std::vector<uint8>& Code) = 0;
	virtual FRHIGeometryShader* RHICreateGeometryShaderWithStreamOutput() { return NULL; } //Todo:Later

	virtual FRHIComputeShader* RHICreateComputeShader(const std::vector<uint8>& Code) = 0;

	virtual FRHIPixelShader* RHICreatePixelShader(const std::vector<uint8>& Code) = 0;
	virtual void FlushPendingLogs() {}

	virtual FRHIBoundShaderState* RHICreateBoundShaderState(FRHIVertexDeclaration* InVertexDeclaration , FRHIVertexShader* InVertexShader , FRHIHullShader* InHullShader ,FRHIDomainShader* InDomainShader,FRHIGeometryShader* InGeometryShader , FRHIPixelShader* InPixelShader) = 0;

	//Resource-----------------------------------------------------Fence CPU GPU Flush
	virtual FRHIComputeFence* RHICreateComputeFence() {
		return new FRHIComputeFence;
	}

	//Resource-----------------------------------------------------Buffer CBuffer Index  Vertex Structure Buffer
	virtual FRHIUniformBuffer* RHICreateUniformBuffer(const void* Contents, const FRHIUniformBufferLayout& Layout, EUniformBufferUsage Usage) = 0;// CBuffer
	virtual FRHIIndexBuffer* RHICreateIndexBuffer(uint32 Stride, uint32 Size, uint32 InUsage, FRHIResourceCreateInfo& CreateInfo) = 0;
	virtual void* RHILockIndexBuffer(FRHIIndexBuffer* IndexBuffer, uint32 Offset, uint32 Size, EResourceLockMode LockMode) = 0;//Flush
	virtual void RHIUnlockIndexBuffer(FRHIIndexBuffer* IndexBuffer) = 0;//Flush
	virtual FRHIVertexBuffer* RHICreateVertexBuffer(uint32 Size, uint32 InUsage, FRHIResourceCreateInfo& CreateInfo) = 0;
	virtual void* RHILockVertexBuffer(FRHIVertexBuffer* VertexBuffer, uint32 Offset, uint32 Size, EResourceLockMode LockMode) = 0;//Flush
	virtual void RHIUnlockVertexBuffer(FRHIVertexBuffer* VertexBuffer) = 0;//Flush
	virtual void RHICopyVertexBuffer(FRHIVertexBuffer* SourceBufferRHI, FRHIVertexBuffer* DestBufferRHI) = 0;
	virtual FRHIStructureBuffer* RHICreateStructureBuffer(uint32 Stride, uint32 Size, uint32 InUsage, FRHIResourceCreateInfo& CreateInfo) = 0;
	virtual void* RHILockStructureBuffer(FRHIStructureBuffer* StructureBuffer, uint32 Offset, uint32 Size, EResourceLockMode LockMode) = 0;//Flush
	virtual void RHIUnlockStructureBuffer(FRHIStructureBuffer* StructureBuffer) = 0;//Flush

	//Resource-----------------------------------------------------Texture 
	virtual FRHITexture2D* RHICreateTexture2D(uint32 SizeX, uint32 SizeY, uint8 Format, uint32 NumMips, uint32 NumSamples, uint32 Flags, FRHIResourceCreateInfo& CreateInfo) = 0;
	virtual FRHITexture2D* RHIAsyncCreateTexture2D(uint32 SizeX, uint32 SizeY, uint8 Format, uint32 NumMips, uint32 Flags, void** InitialMipData, uint32 NumInitialMips) = 0;
	virtual void RHICopySharedMips(FRHITexture2D* DestTexture2D, FRHITexture2D* SrcTexture2D) = 0;
	virtual FRHITexture2DArray* RHICreateTexture2DArray(uint32 SizeX, uint32 SizeY, uint32 SizeZ, uint8 Format, uint32 NumMips, uint32 Flags, FRHIResourceCreateInfo& CreateInfo) = 0;
	virtual FRHITexture3D* RHICreateTexture3D(uint32 SizeX, uint32 SizeY, uint32 SizeZ, uint8 Format, uint32 NumMips, uint32 NumSamples, uint32 Flags, FRHIResourceCreateInfo& CreateInfo) = 0;
	virtual FRHITextureCube* RHICreateTextureCube(uint32 Size, uint8 Format, uint32 NumMips, uint32 Flags, FRHIResourceCreateInfo& CreateInfo) = 0;
	virtual FRHITextureCube* RHICreateTextureCubeArray(uint32 Size, uint32 ArraySize, uint8 Format, uint32 NumMips, uint32 Flags, FRHIResourceCreateInfo& CreateInfo) = 0;

	virtual uint64 RHICalcTexture2DPlatformSize(uint32 SizeX , uint32 SizeY , uint8 Format , uint32 NumMips , uint32 Flag , uint32& OutAlign) = 0;
	virtual uint64 RHICalcTexture3DPlatformSize(uint32 SizeX, uint32 SizeY, uint32 SizeZ,uint8 Format, uint32 NumMips, uint32 Flag, uint32& OutAlign) = 0;
	virtual uint64 RHICalcTextureCubePlatformSize(uint32 Size, uint8 Format, uint32 NumMips, uint32 Flag, uint32& OutAlign) = 0;

	//virtual void RHIGetTextureMemoryStats(FTextureMemoryStats& OutStats) = 0;
	virtual bool RHIGetTextureMemoryVisualizeData(FColor* TextureData , int32 SizeX , int32 SizeY , int32 Pitch , int32 PixelSize) = 0;
	//virtual FRHITextureReference* RHICreateTextureRefrence(FLastRenderTimeContainer* LastRenderTime) = 0;

	//virtual void RHIGetResourceInfo(FRHITexture* Texture , FRHIResourceInfo& OutInfo);
	virtual void RHIGenerateMips(FRHITexture* Texture) = 0;
	virtual uint32 RHIComputeMemorySize(FRHITexture* Texture) = 0;

	//virtual FRHITexture* RHIAsyncReallocateTexture2D(FRHITexture* Texture2D , int32 NewMipCount, int32 NewSizeX , int32 NewSizeY, FThreadSafeCounter* RequestStatus) = 0;
	virtual ETextureReallocationStatus RHIFinalizeAsyncReallocateTexture2D(FRHITexture2D* Texture2D , bool bBlockUntilCompleted) = 0;
	virtual ETextureReallocationStatus RHICancelAsyncReallocateTexture2D(FRHITexture2D* Texture2D, bool bBlockUntilCompleted) = 0;

	virtual void* RHILockTexture2D(FRHITexture2D* Texture ,uint32 MipIndex ,EResourceLockMode LockMode , uint32& DestStride , bool bLockWithinMiptail) = 0;
	virtual void RHIUnlockTexture2D(FRHITexture2D* Texture , uint32 MipIndex , bool bLockWithinMiptail) = 0;
	virtual void* RHILockTexture2DArray(FRHITexture2DArray* TextureArray , uint32 TextureIndex,uint32 MipIndex , EResourceLockMode LockMode , uint32& DestStride , bool bLockWithInMiptail) = 0;
	virtual void RHIUnlockTexture2DArray(FRHITexture2DArray* TextureArray , uint32 TextureIndex , uint32 MipIndex , bool bLockWithMiptail) = 0;
	virtual void* RHILockTextureCubeFace(FRHITextureCube* TextureCube , uint32 FaceIndex , uint32 ArrayIndex , uint32 MipIndex , EResourceLockMode LockMode ,uint32& DestStride , bool bLockWithInMiptail ) = 0;
	virtual void RHIUnLockTextureCubeFace(FRHITextureCube* TextureCube , uint32 FaceIndex , uint32 ArrayIndex , uint32 MipIndex , bool bLockWithinMiptail ) = 0;

	virtual void RHIUpdateTexture2D(FRHITexture2D* Texture2D , uint32 MipLevel ,FUpdateTextureRegion2D& Region2D ,uint32 SourcePitch,uint8* SourceData ) = 0;
	virtual void RHIUpdateTexture3D(FRHITexture3D* Texture3D, uint32 MipLevel, FUpdateTextureRegion3D& Region3D, uint32 SourceRowPitch, uint32 SourceDepthPitch, uint8* SourceData) = 0;

	//ReadSurfaceData
	//virtual void RHIReadSurfaceData(FTextureRHIParamRef Texture, FIntRect Rect, TArray<FColor>& OutData, FReadSurfaceDataFlags InFlags) = 0;
	//virtual void RHIReadSurfaceData(FTextureRHIParamRef Texture, FIntRect Rect, TArray<FLinearColor>& OutData, FReadSurfaceDataFlags InFlags) {}
	//virtual void RHIMapStagingSurface(FTextureRHIParamRef Texture, void*& OutData, int32& OutWidth, int32& OutHeight) = 0;
	//virtual void RHIUnmapStagingSurface(FTextureRHIParamRef Texture) = 0;
	//virtual void RHIReadSurfaceFloatData(FTextureRHIParamRef Texture, FIntRect Rect, TArray<FFloat16Color>& OutData, ECubeFace CubeFace, int32 ArrayIndex, int32 MipIndex) = 0;
	//virtual void RHIRead3DSurfaceFloatData(FTextureRHIParamRef Texture, FIntRect Rect, FIntPoint ZMinMax, TArray<FFloat16Color>& OutData) = 0;

	//Resource-----------------------------------------------------View   ShaderInput
	virtual FRHIUnorderedAccessView* RHICreateUnorderedAccessView(FRHITexture* Texture, uint32 MipLevel) = 0;
	virtual FRHIUnorderedAccessView* RHICreateUnorderedAccessView(FRHIStructureBuffer* StructBuffer, bool bUseUAVCounter, bool bAppendBuffer) = 0;
	virtual FRHIUnorderedAccessView* RHICreateUnorderedAccessView(FRHIVertexBuffer* VertexBuffer, uint8 Format) = 0;
	virtual FRHIShaderResourceView* RHICreateShaderResourceView(FRHIStructureBuffer* StructBuffer) = 0;
	virtual FRHIShaderResourceView* RHICreateShaderResourceView(FRHIVertexBuffer* VertexBuffer, uint32 Stride, uint8 Format) = 0;
	virtual FRHIShaderResourceView* RHICreateShaderResourceView(FRHIIndexBuffer* IndexBuffer) = 0;
	virtual FRHIShaderResourceView* RHICreateShaderResourceView(FRHITexture2D* Texture2D, uint8 MipLevel) = 0;
	virtual FRHIShaderResourceView* RHICreateShaderResourceView(FRHITexture2D* Texture2D, uint8 MipLevel, uint8 NumMipLevels, uint8 Format) = 0;
	virtual FRHIShaderResourceView* RHICreateShaderResourceView(FRHITexture3D* Texture3D, uint8 MipLevel) = 0;
	virtual FRHIShaderResourceView* RHICreateShaderResourceView(FRHITexture2DArray* Texture2DArray, uint8 MipLevel) = 0;
	virtual FRHIShaderResourceView* RHICreateShaderResourceView(FRHITextureCube* TextureCube, uint8 MipLevel) = 0;

	//Query-------------------------------------------------------------------------------------------------------------------
	virtual FRHIRenderQuery* RHICreateRenderQuery(ERenderQueryType QueryType) = 0;
	virtual bool RHIGetRenderQueryResult(FRHIRenderQuery* RenderQuery, uint64& OutResult, bool bWait) = 0;

	//Viewport-----------------------------------------------------------------------------------------------------------------
	virtual FRHIViewport* RHICreateViewport(void* WindowsHandle, uint32 SizeX, uint32 SizeY, bool bIsFullScreen, EPixelFormat PreferredPixelFormat) = 0;
	virtual void RHIResizeViewport(FRHIViewport* Viewport, uint32 SizeX , uint32 SizeY , bool bIsFullScreen) = 0;
	virtual FRHITexture* RHIGetViewportBackBuffer(FRHIViewport* Viewport) = 0;
	virtual void RHIAdvanceFrameForGetViewportBackBuffer() = 0;

	//Multithread--------------------------------------------------------------------------------------------------------------
	virtual void RHIAcquireThreadOwnership() = 0;
	virtual void RHIReleaseThreadOwnership() = 0;
	virtual void RHIFlushResource() = 0;
	//virtual void RHIExecuteCommandList(FRHICommandList* CmdList) = 0;

	//GPU---------------------------------------------------------------------------------------------------------------------
	virtual uint32 RHIGetGPUFrameCycles() = 0;
	virtual void RHITick(float DeltaTime) = 0;
	virtual void RHISuspendRendering() {};
	virtual void RHIResumeRendering() {};
	virtual bool RHIIsRenderingSuspended() { return false; };

protected:
private:
};

extern FDynamicRHI* GDynamicRHI;

FDynamicRHI* PlatformCreateDynamicRHI();