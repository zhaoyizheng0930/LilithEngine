#pragma once
#include "HAL/PlatformIncludes.h"
#include "PixelFormat.h"

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

	uint32 GetStride() { return Stride; }
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
class FRHITexture : public FRHIResource 
{
public:
	FRHITexture(uint32 InNumMips, uint32 InNumSamples, EPixelFormat InFormat, uint32 InFlags)
		:NumMips(InNumMips),
		NumSamples(InNumSamples),
		Flags(InFlags),
		Format(InFormat)
	{

	}

	virtual class FRHITexture2D* GetTexture2D() { return NULL; }
	virtual class FRHITexture2DArray* GetTexture2DArray() { return NULL; }
	virtual class FRHITexture3D* GetTexture3D() { return NULL; }
	virtual class FRHITextureCube* GetTextureCube() { return NULL; }
	virtual class FRHITextureReference* GetTextureReference() { return NULL; }

	virtual uint32 GetNumMips() {
		return NumMips;
	}

	virtual uint32 GetNumSamples() {
		return NumSamples;
	}

	virtual EPixelFormat GetPixelFormat() {
		return Format;
	}

	virtual void* GetTextureBaseRHI()
	{
		return nullptr;
	}

	virtual uint32 GetFlags() { return Flags; }
private:
	uint32 NumMips;
	uint32 NumSamples;
	EPixelFormat Format;
	uint32 Flags;
};

class FRHITextureReference : public FRHITexture 
{
public:
	FRHITextureReference()
		:FRHITexture(0,0, PF_Unknown,0)
	{

	}

	virtual FRHITextureReference* GetTextureReference() { return this; }
protected:
	void SetReferencedTexture(FRHITexture* InTexture)
	{
		RefTexture = InTexture;
	}
private:
	FRHITexture* RefTexture;
};
class FRHITextureReferenceNullImpl : public FRHITextureReference
{
public:
	FRHITextureReferenceNullImpl()
		:FRHITextureReference()
	{}

	void SetReferencedTexture(FRHITexture* InTexture)
	{
		FRHITextureReference::SetReferencedTexture(InTexture);
	}
};
class FRHITextureCube : public FRHITexture
{
public:
	FRHITextureCube(uint32 InSize, uint32 InNumMips, uint32 InNumSamples, EPixelFormat InFormat, uint32 InFlags)
		:FRHITexture(InNumMips, InNumSamples, InFormat, InFlags),
		Size(InSize)
	{

	}

	virtual class FRHITextureCube* GetTextureCube() { return this; }

	uint32 GetSize() const { return Size; }
private:
	uint32 Size;
};
class FRHITexture3D : public FRHITexture 
{
public:
	FRHITexture3D(uint32 InSizeX, uint32 InSizeY, uint32 InSizeZ, uint32 InNumMips, EPixelFormat InFormat, uint32 InFlags )
		:FRHITexture(InNumMips, 1, InFormat , InFlags),
		SizeX(InSizeX),
		SizeY(InSizeY),
		SizeZ(InSizeZ)
	{

	}

	virtual FRHITexture3D* GetTexture3D() { return this; }

	uint32 GetSizeX() { return SizeX; }
	uint32 GetSizeY() { return SizeY; }
	uint32 GetSizeZ() { return SizeZ; }
private:
	uint32 SizeX;
	uint32 SizeY;
	uint32 SizeZ;
};
class FRHITexture2DArray : public FRHITexture 
{
public:
	FRHITexture2DArray(uint32 InSizeX, uint32 InSizeY, uint32 InSizeZ, uint32 InNumMips, uint32 InNumSamples,EPixelFormat InFormat, uint32 InFlags)
		:FRHITexture(InNumMips, InNumSamples, InFormat, InFlags),
		SizeX(InSizeX),
		SizeY(InSizeY),
		SizeZ(InSizeZ)
	{

	}

	virtual FRHITexture2DArray* GetTexture2DArray() { return this; }

	uint32 GetSizeX() const { return SizeX; }

	uint32 GetSizeY() const { return SizeY; }

	uint32 GetSizeZ() const { return SizeZ; }
private:
	uint32 SizeX;
	uint32 SizeY;
	uint32 SizeZ;
};
class FRHITexture2D : public FRHITexture 
{
public:
	FRHITexture2D(uint32 InSizeX, uint32 InSizeY , uint32 InNumMips, uint32 InNumSamples, EPixelFormat InFormat, uint32 InFlags)
		:FRHITexture(InNumMips , InNumSamples , InFormat, InFlags),
		SizeX(InSizeX),
		SizeY(InSizeY)
	{

	}

	virtual FRHITexture2D* GetTexture2D() { return this; }

	uint32 GetSizeX() { return SizeX; }
	uint32 GetSizeY() { return SizeY; }
private:
	uint32 SizeX;
	uint32 SizeY;
};

//Others
class FRHIComputeFence : public FRHIResource {};
class FRHIViewport : public FRHIResource 
{

};
class FRHICustomPresent : public FRHIResource 
{
public:
	FRHICustomPresent(FRHIViewport* InViewportRHI)
		:ViewportRHI(InViewportRHI)
	{

	}

	virtual void OnBackBufferResize() = 0;

	virtual bool Present(int32& InOutSyncInterval) = 0;

private:
	FRHIViewport* ViewportRHI;
};
class FRHIRenderQuery : public FRHIResource {};

//RTV
class FRHIRenderTargetView 
{
public:
	FRHITexture* Texture;
	uint32 MipIndex;

	uint32 ArraySliceIndex;

	FRHIRenderTargetView(FRHITexture* InTexture)
		:Texture(InTexture)
	{

	}

};

class FExclusiveDepthStencil 
{
public:
	enum Type
	{
		// don't use those directly, use the combined versions below
		// 4 bits are used for depth and 4 for stencil to make the hex value readable and non overlapping
		DepthNop = 0x00,
		DepthRead = 0x01,
		DepthWrite = 0x02,
		DepthMask = 0x0f,
		StencilNop = 0x00,
		StencilRead = 0x10,
		StencilWrite = 0x20,
		StencilMask = 0xf0,

		// use those:
		DepthNop_StencilNop = DepthNop + StencilNop,
		DepthRead_StencilNop = DepthRead + StencilNop,
		DepthWrite_StencilNop = DepthWrite + StencilNop,
		DepthNop_StencilRead = DepthNop + StencilRead,
		DepthRead_StencilRead = DepthRead + StencilRead,
		DepthWrite_StencilRead = DepthWrite + StencilRead,
		DepthNop_StencilWrite = DepthNop + StencilWrite,
		DepthRead_StencilWrite = DepthRead + StencilWrite,
		DepthWrite_StencilWrite = DepthWrite + StencilWrite,
	};

private:
	Type Value;

public:
	FExclusiveDepthStencil(Type InValue = DepthNop_StencilNop)
		: Value(InValue)
	{
	}

	uint32 GetIndex() const
	{
		// Note: The array to index has views created in that specific order.

		// we don't care about the Nop versions so less views are needed
		// we combine Nop and Write
		switch (Value)
		{
		case DepthWrite_StencilNop:
		case DepthNop_StencilWrite:
		case DepthWrite_StencilWrite:
		case DepthNop_StencilNop:
			return 0; // old DSAT_Writable

		case DepthRead_StencilNop:
		case DepthRead_StencilWrite:
			return 1; // old DSAT_ReadOnlyDepth

		case DepthNop_StencilRead:
		case DepthWrite_StencilRead:
			return 2; // old DSAT_ReadOnlyStencil

		case DepthRead_StencilRead:
			return 3; // old DSAT_ReadOnlyDepthAndStencil
		}
		// should never happen
		return -1;
	}

	inline void SetDepthWrite()
	{
		Value = (Type)(ExtractStencil() | DepthWrite);
	}
	inline void SetStencilWrite()
	{
		Value = (Type)(ExtractDepth() | StencilWrite);
	}

	inline void SetDepthStencilWrite(bool bDepth, bool bStencil)
	{
		Value = DepthNop_StencilNop;

		if (bDepth)
		{
			SetDepthWrite();
		}
		if (bStencil)
		{
			SetStencilWrite();
		}
	}

	inline bool IsDepthWrite() const
	{
		return ExtractDepth() == DepthWrite;
	}
private:
	inline Type ExtractDepth() const
	{
		return (Type)(Value & DepthMask);
	}

	inline Type ExtractStencil() const
	{
		return (Type)(Value & StencilMask);
	}

};

class FRHIDepthRenderTargetView
{
public:
	FRHITexture* Texture;

	FExclusiveDepthStencil GetDepthStencilAccess() const { return DepthStencilAccess; }

private:
	FExclusiveDepthStencil DepthStencilAccess;
};

class FRHISetRenderTargetsInfo : public FRHIResource {};

//View
class FRHIUnorderedAccessView : public FRHIResource {};
class FRHIShaderResourceView : public FRHIResource {};
