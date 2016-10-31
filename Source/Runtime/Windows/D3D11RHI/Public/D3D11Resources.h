#pragma once
#include "RHIResource.h"
#include "ShaderCore.h"

/**
* A rendering resource which is owned by the rendering thread.
*/

class FD3D11BaseShaderResource
{
public:
protected:
private:
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
	FD3D11BoundShaderState() { }
	FD3D11BoundShaderState(FRHIVertexDeclaration* InVertexDeclaration, FRHIVertexShader* InVertexShader, FRHIHullShader* InHullShader, FRHIDomainShader* InDomainShader, FRHIGeometryShader* InGeometryShader, FRHIPixelShader* InPixelShader, ID3D11Device* Direct3DDevice);
	~FD3D11BoundShaderState() {}


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
protected:
private:
};

class FD3D11Texture2D :public FRHITexture2D, public FD3D11TextureBase
{

};

class FD3D11Texture2DArray :public FRHITexture2DArray, public FD3D11TextureBase
{

};

class FD3D11Texture2D :public FRHITexture2D, public FD3D11TextureBase
{

};

class FD3D11Texture2D :public FRHITexture2D, public FD3D11TextureBase
{

};

class FD3D11Texture2D :public FRHITexture2D, public FD3D11TextureBase
{

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