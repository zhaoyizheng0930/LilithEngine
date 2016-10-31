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
struct FRHIUniformBufferLayout
{
	uint32 ConstantBufferSize;
	uint32 ResourceOffset;
	std::vector<uint8> Resource;//EUniformBufferBaseType
	std::string Name;
};
class FRHIUniformBuffer : public FRHIResource 
{
public:
	FRHIUniformBuffer(const FRHIUniformBufferLayout& InLayout)
		: Layout(&InLayout)
	{}

	const FRHIUniformBufferLayout* Layout;
};
class FRHIIndexBuffer : public FRHIResource
{
public:
	FRHIIndexBuffer(uint32 InStride,uint32 InSize,uint32 InUsage)
		:Stride(InStride),
		Size(InSize),
		Usage(InUsage)
	{

	}
private:
	uint32 Stride;
	uint32 Size;
	uint32 Usage;
};
class FRHIVertexBuffer : public FRHIResource 
{
public:
	FRHIVertexBuffer(uint32 InSize , uint32 InUsage)
		:Size(InSize),
		Usage(InUsage)
	{

	}

	uint32 Size;
	uint32 Usage;
};
class FRHIStructureBuffer : public FRHIResource
{
public:
	FRHIStructureBuffer(uint32 InSize , uint32 InUsage , uint32 InStride)
		:Size(InSize),
		Usage(InUsage),
		Stride(InStride)
	{

	}

	uint32 Size;
	uint32 Usage;
	uint32 Stride;
};

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
class FRHIRenderQuery : public FRHIResource {};

//RTV
class FRHIRenderTargetView : public FRHIResource {};
class FExclusiveDepthStencil : public FRHIResource {};
class FRHIDepthRenderTargetView : public FRHIResource {};
class FRHISetRenderTargetsInfo : public FRHIResource {};

//View
class FRHIUnorderedAccessView : public FRHIResource {};
class FRHIShaderResourceView : public FRHIResource {};
