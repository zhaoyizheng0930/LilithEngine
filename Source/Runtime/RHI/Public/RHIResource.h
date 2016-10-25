#pragma once
#include "HAL/PlatformIncludes.h"
class FRHIResource
{
public:
protected:
private:
};

//State Block
class FRHISamplerState : public FRHIResource {};
class FRHIRasterizerState : public FRHIResource {};
class FRHIDepthStencilState : public FRHIResource {};
class FRHIBlendState : public FRHIResource {};

//shader bindings
class FRHIVertexDeclaration : public FRHIResource {};
class FRHIBoundShaderState : public FRHIResource {};

//Shader
class FRHIShader : public FRHIResource {};
class FRHIVertexShader : public FRHIShader {};
class FRHIHullShader : public FRHIShader {};
class FRHIDomainShader : public FRHIShader {};
class FRHIGeometryShader : public FRHIShader {};
class FRHIComputeShader : public FRHIShader {};
class FRHIPixelShader : public FRHIShader {};

//Buffer
struct FRHIUniformBufferLayout {};
class FRHIUniformBuffer : public FRHIResource {};
class FRHIIndexBuffer : public FRHIResource {};
class FRHIVertexBuffer : public FRHIResource {};
class FRHIStructureBuffer : public FRHIResource {};

//Texture
class FRHITexture : public FRHIResource {};
class FRHITextureReference : public FRHITexture {};
class FRHITextureReferenceNullImpl : public FRHITextureReference {};
class FRHITextureCube : public FRHITexture {};
class FRHITexture3D : public FRHITexture {};
class FRHITexture2DArray : public FRHITexture {};
class FRHITexture2D : public FRHITexture {};

//Others
class FRHIComputeFence : public FRHIResource {};
class FRHIViewport : public FRHIResource {};
class FRHICustomPresent : public FRHIResource {};

//RTV
class FRHIRenderTargetView : public FRHIResource {};
class FExclusiveDepthStencil : public FRHIResource {};
class FRHIDepthRenderTargetView : public FRHIResource {};
class FRHISetRenderTargetsInfo : public FRHIResource {};
