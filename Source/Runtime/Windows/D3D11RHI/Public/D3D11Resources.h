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


class FD3D11TextureBase :public FD3D11BaseShaderResource
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