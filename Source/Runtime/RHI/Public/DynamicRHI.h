#pragma once
#include "RHIResource.h"
#include "RHIDefinitions.h"

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

	//Resource-----------------------------------------------------Shader

protected:
private:
};

extern FDynamicRHI* GDynamicRHI;

FDynamicRHI* PlatformCreateDynamicRHI();