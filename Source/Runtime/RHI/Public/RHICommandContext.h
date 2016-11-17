#pragma once
#include "Math/Color.h"

class IRHIComputeContext
{
public:
	//Query
	virtual void RHIWaitComputeFence(FRHIComputeFence* InFence) = 0;

	//Shader--------------------------------------------------------ComputeShader
	virtual void RHISetComputeShader(FRHIComputeShader* ComputerShader) = 0;

	virtual void RHIDispatchComputeShader(uint32 ThreadGroupCountX, uint32 ThreadGroupCountY, uint32 ThreadGroupCountZ) = 0;

	virtual void RHIDispatchIndirectComputeShader(FRHIVertexBuffer* ArgumentBuffer, uint32 ArgumentOffset) = 0;

	virtual void RHISetAsyncComputeBudget(EAsyncComputeBudget Budget) = 0;

	virtual void RHITransitionResources(EResourceTransitionAccess TransitionType, EResourceTransitionPipeline TransitionPipeline, FRHIUnorderedAccessView* InUAV, int32 NumUAVs, FRHIComputeFence* WriteComputerFence) = 0;

	virtual void RHISetShaderTexture(FRHIComputeShader* ComputeShader, uint32 TextureIndex, FRHITexture* NewTexture) = 0;

	virtual void RHISetShaderSampler(FRHIComputeShader* ComputeShader, uint32 SamplerIndex, FRHISamplerState* SamplerState) = 0;

	virtual void RHISetUAVParam(FRHIComputeShader* ComputeShader, uint32 UAVIndex, FRHIUnorderedAccessView* UAV) = 0;

	virtual void RHISetUAVParam(FRHIComputeShader* ComputeShader, uint32 UAVIndex, FRHIUnorderedAccessView* UAV, uint32 InitalCount) = 0;

	virtual void RHISetShaderResourceViewParam(FRHIComputeShader* ComputeShader, uint32 SRVIndex, FRHIShaderResourceView* SRV) = 0;

	virtual void RHISetShaderUniformBuffer(FRHIComputeShader* ComputeShader, uint32 BufferIndex, FRHIUniformBuffer* UniformBuffer) = 0;

	virtual void RHISetShaderParameter(FRHIComputeShader* ComputeShader, uint32 BufferIndex, uint32 BaseIndex, uint32 NumBytes, const void* NewValue) = 0;

	//Event---------------------------------------------------------------------------------------------------------------------
	virtual void PushEvent(char* Name, FColor Color) = 0;

	virtual void PopEvent() = 0;

	virtual void RHISubmitCommandsHint() = 0;

protected:
private:
};

class IRHICommandContext:public IRHIComputeContext
{
public:
	virtual ~IRHICommandContext()
	{
	}

	//Resource-----------------------------------------------------------------------------------------------------------------ComputeShader
	virtual void RHIWaitComputeFence(FRHIComputeFence* InFence) override
	{
	}

	virtual void RHISetAsyncComputeBudget(EAsyncComputeBudget Budget) override
	{
	}

	virtual void RHIAutomaticCacheFlushAfterComputeShader(bool bEnable) = 0;

	virtual void RHIFlushComputeShaderCache() = 0;

	//UAV
	virtual void RHIClearUAV(FRHIUnorderedAccessView* UAV, uint32* Values) = 0;
	//Resolve
	virtual void RHICopyToResolveTarget(FRHITexture* SourceTexture , FRHITexture* DestTexture , bool bKeepOriginalSurface ,const FResolveParams& ResolveParam ) = 0;

	virtual void RHITransitionResources(EResourceTransitionAccess TransitionType, FRHITexture** InTextures, int32 NumTextures)
	{
		if (TransitionType == EResourceTransitionAccess::EReadable)
		{
			const FResolveParams ResolveParams;
			for (int32 i = 0; i < NumTextures; ++i)
			{
				RHICopyToResolveTarget(InTextures[i], InTextures[i], true, ResolveParams);
			}
		}
	}

	virtual void RHITransitionResources(EResourceTransitionAccess TransitionType, EResourceTransitionPipeline TransitionPipeline, FRHIUnorderedAccessView* InUAVs, int32 NumUAVs, FRHIComputeFence* WriteComputeFence)
	{
		//ZYZ_TODO:Support later
		/*if (WriteComputeFence)
		{
			WriteComputeFence->WriteFence();
		}*/
	}

	void RHITransitionResources(EResourceTransitionAccess TransitionType, EResourceTransitionPipeline TransitionPipeline, FRHIUnorderedAccessView* InUAVs, int32 NumUAVs)
	{
		RHITransitionResources(TransitionType, TransitionPipeline, InUAVs, NumUAVs, nullptr);
	}

	//Resource----------------------------------------------------------------------------------------------------Query
	virtual void RHIBeginRenderQuery(FRHIRenderQuery* RenderQuery) = 0;

	virtual void RHIEndRenderQuery(FRHIRenderQuery* RenderQuery) = 0;

	virtual void RHIBeginOcclusionQueryBatch() = 0;

	virtual void RHIEndOcclusionQueryBatch() = 0;

	//Commands Hint
	virtual void RHISubmitCommandsHint() = 0;

	//ResourceBind----------------------------------------------------------------------------------------------------Viewport
	virtual void RHISetMultipleViewPorts(uint32 Count, FViewportBound* Data) = 0;

	virtual void RHIBeginDrawingViewport(FRHIViewport* Viewport , FRHITexture* RenderTarget) = 0;

	virtual void RHIEndDrawingViewport(FRHIViewport* Viewport , bool bPresent , bool bLockVsync) = 0;

	//Frame-------------------------------------------------------------------------------------------------------
	virtual void RHIBeginFram() = 0;

	virtual void RHIEndFram() = 0;

	//Scene-------------------------------------------------------------------------------------------------------
	virtual void RHIBeginScene() = 0;

	virtual void RHIEndScene() = 0;

	//ResourceBind------------------------------------------------------------------------------------------------
	virtual void RHISetStreamSource(uint32 StreamIndex, FRHIVertexBuffer* VertexBuffer, uint32 Stride, uint32 Offset) = 0;

	virtual void RHISetRasterizerState(FRHIRasterizerState* RasterizerState) = 0;

	virtual void RHISetViewport(uint32 MinX, uint32 MinY, float MinZ, uint32 MaxX, uint32 MaxY, float MaxZ) = 0;

	virtual void RHISetStereoViewport(uint32 LeftMinX, uint32 RightMinX, uint32 MinY, float MinZ, uint32 LeftMaxX, uint32 RightMaxX, uint32 MaxY, float MaxZ) = 0;//Seems like use for VR

	virtual void RHISetScissorRect(bool bEnable, uint32 MinX,uint32 MinY, uint32 MaxX,uint32 MaxY) = 0;

	virtual void RHISetBoundShaderState(FRHIBoundShaderState* BoundShaderState) = 0;

	//ResourceBind------------------------------------------------------------------------------------------------SRV Set
	virtual void RHISetShaderTexture(FRHIVertexShader* VertexShader, uint32 TextureIndex, FRHITexture* NewTexture) = 0;

	virtual void RHISetShaderTexture(FRHIHullShader* HullShader, uint32 TextureIndex, FRHITexture* NewTexture) = 0;

	virtual void RHISetShaderTexture(FRHIDomainShader* DomainShader, uint32 TextureIndex, FRHITexture* NewTexture) = 0;

	virtual void RHISetShaderTexture(FRHIGeometryShader* GeometryShader, uint32 TextureIndex, FRHITexture* NewTexture) = 0;

	virtual void RHISetShaderTexture(FRHIComputeShader* ComputeShader, uint32 TextureIndex, FRHITexture* NewTexture) = 0;

	virtual void RHISetShaderTexture(FRHIPixelShader* PixelShader, uint32 TextureIndex, FRHITexture* NewTexture) = 0;

	//ResourceBind------------------------------------------------------------------------------------------------SRV Sampler Set
	virtual void RHISetShaderSampler(FRHIVertexShader* VertexShader , uint32 SamplerIndex , FRHISamplerState* NewState) = 0;

	virtual void RHISetShaderSampler(FRHIHullShader* HullShader, uint32 SamplerIndex, FRHISamplerState* NewState) = 0;

	virtual void RHISetShaderSampler(FRHIDomainShader* DomainShader, uint32 SamplerIndex, FRHISamplerState* NewState) = 0;

	virtual void RHISetShaderSampler(FRHIGeometryShader* GeometryShader, uint32 SamplerIndex, FRHISamplerState* NewState) = 0;

	virtual void RHISetShaderSampler(FRHIComputeShader* ComputeShader, uint32 SamplerIndex, FRHISamplerState* NewState) = 0;

	virtual void RHISetShaderSampler(FRHIPixelShader* PixelShader, uint32 SamplerIndex, FRHISamplerState* NewState) = 0;

	//ResourceBind------------------------------------------------------------------------------------------------UAV
	virtual void RHISetUAVParameter(FRHIComputeShader* ComputeShader , uint32 UAVIndex , FRHIUnorderedAccessView* UAV) = 0;

	virtual void RHISetUAVParameter(FRHIComputeShader* ComputeShader, uint32 UAVIndex, FRHIUnorderedAccessView* UAV , uint32 InitialCount) = 0;

	//ResourceBind------------------------------------------------------------------------------------------------SRV
	virtual void RHISetShaderResourceViewParameter(FRHIVertexShader* VertexShader, uint32 SRVIndex, FRHIShaderResourceView* SRV) = 0;

	virtual void RHISetShaderResourceViewParameter(FRHIHullShader* HullShader, uint32 SRVIndex, FRHIShaderResourceView* SRV) = 0;

	virtual void RHISetShaderResourceViewParameter(FRHIDomainShader* DomainShader, uint32 SRVIndex, FRHIShaderResourceView* SRV) = 0;

	virtual void RHISetShaderResourceViewParameter(FRHIGeometryShader* GeometryShader, uint32 SRVIndex, FRHIShaderResourceView* SRV) = 0;

	virtual void RHISetShaderResourceViewParameter(FRHIComputeShader* ComputeShader, uint32 SRVIndex, FRHIShaderResourceView* SRV) = 0;

	virtual void RHISetShaderResourceViewParameter(FRHIPixelShader* PixelShader, uint32 SRVIndex, FRHIShaderResourceView* SRV) = 0;

	//ResourceBind------------------------------------------------------------------------------------------------C Buffer
	virtual void RHISetShaderUniformBuffer(FRHIVertexShader* VertexShader, uint32 BufferIndex, FRHIUniformBuffer* Buffer) = 0;

	virtual void RHISetShaderUniformBuffer(FRHIHullShader* HullShader, uint32 BufferIndex, FRHIUniformBuffer* Buffer) = 0;

	virtual void RHISetShaderUniformBuffer(FRHIDomainShader* DomainShader, uint32 BufferIndex, FRHIUniformBuffer* Buffer) = 0;

	virtual void RHISetShaderUniformBuffer(FRHIGeometryShader* GeometryShader, uint32 BufferIndex, FRHIUniformBuffer* Buffer) = 0;

	virtual void RHISetShaderUniformBuffer(FRHIComputeShader* ComputeShader, uint32 BufferIndex, FRHIUniformBuffer* Buffer) = 0;

	virtual void RHISetShaderUniformBuffer(FRHIPixelShader* PixelShader, uint32 BufferIndex, FRHIUniformBuffer* Buffer) = 0;

	//ResourceBind------------------------------------------------------------------------------------------------Shader Param
	virtual void RHISetShaderParameter(FRHIVertexShader* VertexShader, uint32 BufferIndex, uint32 BaseIndex , uint32 NumBytes , const void* NewValue) = 0;

	virtual void RHISetShaderParameter(FRHIHullShader* HullShader, uint32 BufferIndex, uint32 BaseIndex, uint32 NumBytes, const void* NewValue) = 0;

	virtual void RHISetShaderParameter(FRHIDomainShader* DomainShader, uint32 BufferIndex, uint32 BaseIndex, uint32 NumBytes, const void* NewValue) = 0;

	virtual void RHISetShaderParameter(FRHIGeometryShader* GeometryShader, uint32 BufferIndex, uint32 BaseIndex, uint32 NumBytes, const void* NewValue) = 0;

	virtual void RHISetShaderParameter(FRHIComputeShader* ComputeShader, uint32 BufferIndex, uint32 BaseIndex, uint32 NumBytes, const void* NewValue) = 0;

	virtual void RHISetShaderParameter(FRHIPixelShader* PixelShader, uint32 BufferIndex, uint32 BaseIndex, uint32 NumBytes, const void* NewValue) = 0;

	//ResourceBind------------------------------------------------------------------------------------------------State Set
	virtual void RHISetDepthStencilState(FRHIDepthStencilState* NewState, uint32 StencilRef) = 0;

	virtual void RHISetBlendState(FRHIBlendState* NewState, const FLinearColor& BlendFactor) = 0;

	//ResourceBind------------------------------------------------------------------------------------------------RTV
	virtual void RHISetRenderTarget(uint32 NumSimulataneousRenderTargets,const FRHIRenderTargetView* NewRenderTarget ,const FRHIDepthRenderTargetView* NewDepthStencilTarget ,uint32 NumUAVs , FRHIUnorderedAccessView** UAVs ) = 0;

	virtual void RHISetRenderTargetAndClear(FRHISetRenderTargetsInfo* RenderTargetInfo) = 0;

	virtual void RHIBindClearMRTValues(bool bClearColor, bool bClearDepth, bool bClearStencil) {}

	//DrawCall-----------------------------------------------------------------------------------------------------
	virtual void RHIDrawPrimitive(uint32 PrimitiveType , uint32 BaseVertexIndex , uint32 NumPrimitives , uint32 NumInstances) = 0;

	virtual void RHIDrawPrimitiveIndirect(uint32 PrimitiveType, FRHIVertexBuffer* ArgumentBuffer , uint32 ArgumentOffset) = 0;

	virtual void RHIDrawIndexIndirect(FRHIIndexBuffer* IndexBuffer , uint32 PrimitiveType , FRHIStructureBuffer* StructureBuffer ,int32 DrawArgumentIndex , uint32 NumInstances ) = 0;

	virtual void RHIDrawIndexedPrimitive(FRHIIndexBuffer IndexBuffer, uint32 PrimitiveType, int32 BaseVertexIndex, uint32 FirstInstance, uint32 NumVertices, uint32 StartIndex, uint32 NumPrimitives, uint32 NumInstances) = 0;

	virtual void RHIDrawIndexedPrimitiveIndirect(uint32 PrimitiveType, FRHIIndexBuffer* IndexBuffer, FRHIVertexBuffer* ArgumentBuffer, uint32 ArgumentOffset) = 0;


	virtual void RHIBeginDrawPrimitiveUP(uint32 PrimitiveType, uint32 NumPrimitives, uint32 NumVertices, uint32 VertexDataStride, void*& OutVertexData) = 0;

	virtual void RHIEndDrawPrimitiveUP() = 0;

	virtual void RHIBeginDrawIndexedPrimitiveUP(uint32 PrimitiveType, uint32 NumPrimitives, uint32 NumVertices, uint32 VertexDataStride, void*& OutVertexData, uint32 MinVertexIndex, uint32 NumIndices, uint32 IndexDataStride, void*& OutIndexData) = 0;

	virtual void RHIEndDrawIndexedPrimitiveUP() = 0;

	//Others-----------------------------------------------------------------------------------------------------
	virtual void RHIClear(bool bClearColor/*, const FLinearColor& Color*/, bool bClearDepth, float Depth, bool bClearStencil, uint32 Stencil/*, FIntRect ExcludeRect*/) = 0;

	virtual void RHIClearMRT(bool bClearColor, int32 NumClearColors/*, const FLinearColor* ColorArray*/, bool bClearDepth, float Depth, bool bClearStencil, uint32 Stencil/*, FIntRect ExcludeRect*/) = 0;

	virtual void RHIEnableDepthBoundTest(bool bEnable, float MinDepth , float MaxDepth) = 0;

	virtual void RHIPushEvent(const char* Name , FColor Color) = 0;

	virtual void RHIPopEvent() = 0;

	virtual void RHIUpdateTextureReference(FRHITextureReference* Texture, FRHITexture* NewTexture) = 0;
protected:
private:
};