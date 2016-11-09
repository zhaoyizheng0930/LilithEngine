#pragma once
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

	virtual void RHITransitionResource(EResourceTransitionAccess TransitionType, EResourceTransitionPipeline TransitionPipeline, FRHIUnorderedAccessView* InUAV, int32 NumUAVs, FRHIComputeFence* WriteComputerFence) = 0;

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

	//Resource----------------------------------------------------------------------------------------------------Viewport
	virtual void RHISetMultipleViewPorts(uint32 Count, FViewportBound* ViewPort) = 0;

	virtual void RHIBeginDrawingViewport(FRHIViewport* Viewport , FRHITexture* RenderTargetRHI) = 0;

	virtual void RHIEndDrawingViewport(FRHIViewport* Viewport , bool bPresent , bool bLockVsync) = 0;

	//Frame-------------------------------------------------------------------------------------------------------
	virtual void RHIBeginFram() = 0;

	virtual void RHIEndFram() = 0;

	//Scene-------------------------------------------------------------------------------------------------------
	virtual void RHIBeginScene() = 0;

	virtual void RHIEndScene() = 0;

	//ResourceBind------------------------------------------------------------------------------------------------
	virtual void RHISetStreamSource(uint32 StreamIndex, FRHIVertexBuffer* VertexBuffer, uint32 Stride, uint32 Offset) = 0;


protected:
private:
};