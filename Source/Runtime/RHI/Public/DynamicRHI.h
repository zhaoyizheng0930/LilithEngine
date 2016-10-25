#pragma once
#include "RHIResource.h"
#include "RHI.h"


class FDynamicRHI
{
public:
	virtual ~FDynamicRHI() {}

	virtual void Init() = 0;

	virtual void PostInit() {}

	virtual void Shutdown() = 0;

public:
	/////// RHI Methods
	virtual void RHISetScissorRect(bool bEnable, uint32 MinX, uint32 MinY, uint32 MaxX, uint32 MaxY) = 0;

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
	virtual FRHIGeometryShader* RHICreateGeometryShaderWithStreamOutput() {} //Todo:Later

	virtual FRHIComputeShader* RHICreateComputeShader(const std::vector<uint8>& Code) = 0;

	virtual FRHIPixelShader* RHICreatePixelShader(const std::vector<uint8>& Code) = 0;
	virtual void FlushPendingLogs() {}

	virtual FRHIBoundShaderState* RHICreateBoundShaderState(FRHIVertexDeclaration* InVertexDeclaration , FRHIVertexShader* InVertexShader , FRHIHullShader* InHullShader ,FRHIDomainShader* InDomainShader,FRHIGeometryShader* InGeometryShader , FRHIPixelShader* InPixelShader) = 0;

	//Resource-----------------------------------------------------Fence
	virtual FRHIComputeFence* RHICreateComputeFence() {
		return new FRHIComputeFence;
	}
	//Resource-----------------------------------------------------Buffer

protected:
private:
};

extern FDynamicRHI* GDynamicRHI;

FDynamicRHI* PlatformCreateDynamicRHI();