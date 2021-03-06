#pragma once
#include "RHIResource.h"
#include "ShaderCore.h"
#include "D3D11Definitions.h"
#include "BoundShaderStateCache.h"

/**
* A rendering resource which is owned by the rendering thread.
*/

class FD3D11BaseShaderResource
{
public:
	FD3D11BaseShaderResource()
	{
		bDirty = false;
		LastFrameWritten = -1;
	}

	void SetDirty(bool bInDirty, uint32 CurrentFrame)
	{
		bDirty = bInDirty;
		LastFrameWritten = CurrentFrame;
	}

	bool IsDirty() const
	{
		return bDirty;
	}
	
	void SetCurrentGPUAccess(EResourceTransitionAccess Access)
	{
		if (Access == EResourceTransitionAccess::EReadable)
		{
			bDirty = false;
		}
		CurrentGPUAccess = Access;
	}

	EResourceTransitionAccess GetCurrentGPUAccess() const
	{
		return CurrentGPUAccess;
	}

	uint32 GetLastFrameWritten() const
	{
		return LastFrameWritten;
	}
protected:
private:
	bool bDirty;

	uint32 LastFrameWritten;

	EResourceTransitionAccess CurrentGPUAccess;
};

class FD3D11VertexDeclaration:public FRHIVertexDeclaration
{
public:
	std::vector<D3D11_INPUT_ELEMENT_DESC> VertexElements;

	FD3D11VertexDeclaration(std::vector<D3D11_INPUT_ELEMENT_DESC>& InVertexElements) :VertexElements(InVertexElements)
	{

	}
protected:
private:
};

struct FD3D11ShaderResourceTable : public FBaseShaderResourceTable
{
	std::vector<uint32> TextureMap;
};

struct FD3D11ShaderData
{
	FD3D11ShaderResourceTable			ShaderResourceTable;
	std::vector<std::string>			UniformBuffers;
	bool						bShaderNeedsGlobalConstantBuffer;
};

class FD3D11VertexShader :public FRHIVertexShader, public FD3D11ShaderData
{
public:
	enum { StaticFrequency = SF_Vertex };
	ID3D11VertexShader* Resource;

	std::vector<uint8> Code;

	int Offset;
protected:
private:
};

class FD3D11HullShader :public FRHIHullShader, public FD3D11ShaderData
{
public:
	enum { StaticFrequency = SF_Hull };
	ID3D11HullShader* Resource;
protected:
private:
};

class FD3D11DomainShader :public FRHIDomainShader, public FD3D11ShaderData
{
public:
	enum { StaticFrequency = SF_Domain };
	ID3D11DomainShader* Resource;
protected:
private:
};

class FD3D11GeometryShader :public FRHIGeometryShader, public FD3D11ShaderData
{
public:
	enum { StaticFrequency = SF_Geometry };
	ID3D11GeometryShader* Resource;
protected:
private:
};

class FD3D11ComputeShader :public FRHIComputeShader, public FD3D11ShaderData
{
public:
	enum { StaticFrequency = SF_Compute };
	ID3D11ComputeShader* Resource;
protected:
private:
};

class FD3D11PixelShader :public FRHIPixelShader, public FD3D11ShaderData
{
public:
	enum { StaticFrequency = SF_Pixel };
	ID3D11PixelShader* Resource;
protected:
private:
};

class FD3D11BoundShaderState : public FRHIBoundShaderState
{
public:
	FD3D11BoundShaderState(FRHIVertexDeclaration* InVertexDeclaration, FRHIVertexShader* InVertexShader, FRHIHullShader* InHullShader, FRHIDomainShader* InDomainShader, FRHIGeometryShader* InGeometryShader, FRHIPixelShader* InPixelShader, ID3D11Device* Direct3DDevice);
	~FD3D11BoundShaderState() {}

	FCachedBoundShaderStateLink CacheLink;

	FD3D11VertexShader* GetVertexShader() { return (FD3D11VertexShader*)CacheLink.GetVertexShader(); }
	FD3D11PixelShader* GetPixelShader() { return (FD3D11PixelShader*)CacheLink.GetPixelShader(); }
	FD3D11HullShader* GetHullShader() { return (FD3D11HullShader*)CacheLink.GetHullShader(); }
	FD3D11DomainShader* GetDomainShader() { return (FD3D11DomainShader*)CacheLink.GetDomainShader(); }
	FD3D11GeometryShader* GetGeometryShader() { return (FD3D11GeometryShader*)CacheLink.GetGeometryShader(); }


	ID3D11InputLayout* InputeLayout;
	ID3D11VertexShader* VertexShader;
	ID3D11HullShader* HullShader;
	ID3D11DomainShader* DomainShader;
	ID3D11GeometryShader* GeometryShader;
	ID3D11PixelShader* PixelShader;

	bool bShaderNeedsGlobalConstantBuffer[SF_NumFrequencies];
private:

};

//struct FRingAllocation
//{
//	ID3D11Buffer* Buffer;
//	void* DataPtr;
//	uint32 Offset;
//	uint32 Size;
//
//	FRingAllocation() : Buffer(NULL) {}
//	inline bool IsValid() const { return Buffer != NULL; }
//};

class FD3D11UniformBuffer:public FRHIUniformBuffer
{
public:
	ID3D11Buffer* Resource;

	//FRingAllocation RingAllocation;

	FD3D11UniformBuffer(class FD3D11DynamicRHI* InD3D11RHI, const FRHIUniformBufferLayout& InLayout, ID3D11Buffer* InResource) :FRHIUniformBuffer(InLayout)
	{

	}
protected:
private:
};


class FD3D11IndexBuffer : public FRHIIndexBuffer, public FD3D11BaseShaderResource
{
public:
	ID3D11Buffer* Resource;

	FD3D11IndexBuffer(ID3D11Buffer* InResource, uint32 InStride, uint32 InSize, uint32 InUsage)
		:FRHIIndexBuffer(InStride , InSize , InUsage),
		Resource(InResource)
	{

	}

protected:
private:
};

class FD3D11VertexBuffer:public FRHIVertexBuffer, public FD3D11BaseShaderResource
{
public:
	FD3D11VertexBuffer(ID3D11Buffer* InResource , uint32 InSize , uint32 InUsage) 
		:FRHIVertexBuffer(InSize , InUsage),
		Resource(InResource)
	{

	}

	ID3D11Buffer* Resource;

protected:
private:
};

class FD3D11StructureBuffer :public FRHIStructureBuffer, public FD3D11BaseShaderResource
{
public:
	FD3D11StructureBuffer(ID3D11Buffer* InResource, uint32 InStride, uint32 InSize, uint32 InUsage) :
		FRHIStructureBuffer(InStride, InSize, InUsage),
		Resource(InResource)

	{

	}

	ID3D11Buffer* Resource;
protected:
private:
};


class FD3D11TextureBase :public FD3D11BaseShaderResource
{
public:
	FD3D11TextureBase(FD3D11DynamicRHI* InD3DRHI,
		ID3D11Resource* InResource,
		ID3D11ShaderResourceView* InShaderResourceView,
		int32 InRTVArraySize,
		bool bInCreatedRTVsPerSlice,
		std::vector<ID3D11RenderTargetView*>& InRenderTargetViews,
		std::vector<ID3D11DepthStencilView*>& InDepthStencilViews)
		:D3DRHI(InD3DRHI),
		Resouece(InResource),
		ShaderResourceView(InShaderResourceView),
		RTVArraySize(InRTVArraySize),
		bCreatedRTVsPerSlice(bInCreatedRTVsPerSlice),
		RenderTargetViews(InRenderTargetViews),
		DepthStencilView(InDepthStencilViews)

	{

	}

	virtual ~FD3D11TextureBase()
	{

	}

	ID3D11Resource* GetResource() {
		return Resouece;
	}

	ID3D11ShaderResourceView* GetShaderResourceView() {
		return ShaderResourceView;
	}
	
	ID3D11RenderTargetView* GetRenderTargetView(int32 MipIndex, int32 ArraySliceIndex)
	{
		int32 ArrayIndex = MipIndex;
		if (bCreatedRTVsPerSlice)
		{
			ArrayIndex = MipIndex * RTVArraySize + ArraySliceIndex;
		}
		if (RenderTargetViews.size() > ArrayIndex)
		{
			return RenderTargetViews[ArrayIndex];
		}
		return NULL;
	}
	
	int GetMemorySize() { return MemorySize; }

	ID3D11DepthStencilView* GetDepthStencilView(FExclusiveDepthStencil AccessType) {
		return DepthStencilView[AccessType.GetIndex()];
	}

	bool HasDepthStencilView() {
		return (DepthStencilView.size() > 0);
	}
protected:
	FD3D11DynamicRHI* D3DRHI;

	int MemorySize;

	FD3D11BaseShaderResource* BaseShaderResource;

	ID3D11Resource* Resouece;

	ID3D11ShaderResourceView* ShaderResourceView;

	std::vector<ID3D11RenderTargetView*> RenderTargetViews;

	bool bCreatedRTVsPerSlice;

	int32 RTVArraySize;

	std::vector<ID3D11DepthStencilView*> DepthStencilView; //Max 4
private:
};

template <class BaseResourceType>
class TD3D11Texture2D :public BaseResourceType, public FD3D11TextureBase
{
public:
	TD3D11Texture2D(FD3D11DynamicRHI* InD3DRHI,
		ID3D11Resource* InResource,
		ID3D11ShaderResourceView* InShaderResourceView,
		int32 InRTVArraySize,
		bool bInCreatedRTVsPerSlice,
		std::vector<ID3D11RenderTargetView*>& InRenderTargetViews,
		std::vector<ID3D11DepthStencilView*>& InDepthStencilViews,
		uint32 InSizeX,
		uint32 InSizeY,
		uint32 InSizeZ,
		uint32 InNumMips,
		uint32 InNumSamples,
		EPixelFormat InFormat,
		uint32 InFlags) :
		FD3D11TextureBase(InD3DRHI, InResource , InShaderResourceView , InRTVArraySize , bInCreatedRTVsPerSlice , InRenderTargetViews , InDepthStencilViews),
		BaseResourceType(InSizeX , InSizeY , InSizeZ , InNumMips , InNumSamples , InFormat , InFlags)
	{

	}
	virtual ~TD3D11Texture2D() {}

	void* Lock(uint32 MipIndex, uint32 ArrayIndex, EResourceLockMode LockMode, uint32& DestStride);

	void Unlock(uint32 MipIndex, uint32 ArrayIndex);

	ID3D11Texture2D* GetResource() const { return (ID3D11Texture2D*)/*FD3D11TextureBase::*/GetResource(); }

	virtual void* GetTextureBaseRHI() override final
	{
		return static_cast<FD3D11TextureBase*>(this);
	}
};

class FD3D11BaseTexture2D : public FRHITexture2D
{
public:
	FD3D11BaseTexture2D(uint32 InSizeX, uint32 InSizeY, uint32 InSizeZ, uint32 InNumMips, uint32 InNumSamples, EPixelFormat InPixelFormat, uint32 InFlags)
		:FRHITexture2D(InSizeX , InSizeY , InNumMips , InNumSamples , InPixelFormat, InFlags)
	{
	}
};

class FD3D11BaseTexture2DArray :public FRHITexture2DArray
{
public:
	FD3D11BaseTexture2DArray(uint32 InSizeX, uint32 InSizeY, uint32 InSizeZ, uint32 InNumMips, uint32 InNumSamples, EPixelFormat InPixelFormat, uint32 InFlags)
		:FRHITexture2DArray(InSizeX , InSizeY , InSizeZ , InNumMips , InNumSamples , InPixelFormat, InFlags)
	{
	}
protected:
private:
};

class FD3D11BaseTextureCube:public FRHITextureCube
{
public:
	FD3D11BaseTextureCube(uint32 InSizeX, uint32 InSizeY, uint32 InSizeZ, uint32 InNumMips, uint32 InNumSamples, EPixelFormat InPixelFormat, uint32 InFlags)
		:FRHITextureCube(InSizeX, InNumMips , InNumSamples , InPixelFormat, InFlags)
	{
	}

	uint32 GetSizeX() const { return GetSize(); }
	uint32 GetSizeY() const { return GetSize(); } //-V524
	uint32 GetSizeZ() const { return 0; }
protected:
private:
};

typedef TD3D11Texture2D<FRHITexture>              FD3D11Texture;
typedef TD3D11Texture2D<FD3D11BaseTexture2D>      FD3D11Texture2D;
typedef TD3D11Texture2D<FD3D11BaseTexture2DArray> FD3D11Texture2DArray;
typedef TD3D11Texture2D<FD3D11BaseTextureCube>    FD3D11TextureCube;

class FD3D11TextureReference:public FRHITextureReference,public FD3D11TextureBase
{
public:
	FD3D11TextureReference(FD3D11DynamicRHI* InD3DRHI):FRHITextureReference(), FD3D11TextureBase(InD3DRHI , NULL , NULL , 0 
		 ,false , std::vector<ID3D11RenderTargetView*>() , std::vector<ID3D11DepthStencilView*>()) {}

	virtual void* GetTextureBaseRHI() override final
	{
		return static_cast<FD3D11TextureBase*>(this);
	}
protected:
private:
};

class FD3D11Texture3D :public FRHITexture3D, public FD3D11TextureBase
{
public:
	FD3D11Texture3D(FD3D11DynamicRHI* InD3DRHI,
		ID3D11Resource* InResource,
		ID3D11ShaderResourceView* InShaderResourceView,
		std::vector<ID3D11RenderTargetView*>& InRenderTargetViews,
		uint32 InSizeX,
		uint32 InSizeY,
		uint32 InSizeZ,
		uint32 InNumMips,
		EPixelFormat InFormat,
		uint32 InFlags)
		:FRHITexture3D(InSizeX , InSizeY , InSizeZ , InNumMips , InFormat , InFlags),
		FD3D11TextureBase(InD3DRHI, InResource ,InShaderResourceView , InRenderTargetViews.size() , false , InRenderTargetViews , std::vector<ID3D11DepthStencilView*>()),
		SizeX(InSizeX),
		SizeY(InSizeY),
		SizeZ(InSizeZ)
	{

	}

	virtual void* GetTextureBaseRHI() override final
	{
		return static_cast<FD3D11TextureBase*>(this);
	}

	uint32 SizeX;
	uint32 SizeY;
	uint32 SizeZ;
};

class FD3D11UnorderedAccessView:public FRHIUnorderedAccessView
{
public:
	FD3D11UnorderedAccessView(ID3D11UnorderedAccessView* InView , FD3D11BaseShaderResource* InResource)
		:View(InView),
		Resource(InResource)
	{

	}

	ID3D11UnorderedAccessView* View;
	FD3D11BaseShaderResource* Resource;
protected:
private:
	
};

class FD3D11ShaderResourceView :public FRHIShaderResourceView
{
public:
	FD3D11ShaderResourceView(ID3D11ShaderResourceView* InView, FD3D11BaseShaderResource* InResource)
		:View(InView),
		Resource(InResource)
	{

	}

	ID3D11ShaderResourceView* View;
	FD3D11BaseShaderResource* Resource;
protected:
private:
};

class FD3D11RenderQuery:public FRHIRenderQuery
{
public:
	ID3D11Query* Resource;

	uint64 Result;

	bool bResultIsCached : 1;

	ERenderQueryType Querytype;

	FD3D11RenderQuery(ID3D11Query* InResource , ERenderQueryType InQuerytype)
		:Resource(InResource),
		Querytype(InQuerytype),
		Result(0),
		bResultIsCached(false)
	{

	}

protected:
private:
};