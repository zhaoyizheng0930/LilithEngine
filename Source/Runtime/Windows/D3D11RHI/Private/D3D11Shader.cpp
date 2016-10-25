#include "D3D11RHIPCH.h"
#include "WindowsD3D11DynamicRHI.h"
#include "D3D11Definitions.h"


FRHIVertexShader* FD3D11DynamicRHI::RHICreateVertexShader(const std::vector<uint8>& Code)
{
	//Add Shader Option Later
	ID3D11VertexShader* D11Shader = nullptr;

	FD3D11VertexShader* Shader = new FD3D11VertexShader;

	Direct3DDevice->CreateVertexShader((void*)Code.data(), Code.size(),NULL , &D11Shader);

	Shader->Resource = D11Shader;

	return Shader;
}

FRHIHullShader* FD3D11DynamicRHI::RHICreateHullShader(const std::vector<uint8>& Code)
{
	//Add Shader Option Later
	ID3D11HullShader* D11Shader = nullptr;

	FD3D11HullShader* Shader = new FD3D11HullShader;

	Direct3DDevice->CreateHullShader((void*)Code.data(), Code.size(), NULL, &D11Shader);

	Shader->Resource = D11Shader;

	return Shader;
}

FRHIDomainShader* FD3D11DynamicRHI::RHICreateDomainShader(const std::vector<uint8>& Code)
{
	//Add Shader Option Later
	ID3D11DomainShader* D11Shader = nullptr;

	FD3D11DomainShader* Shader = new FD3D11DomainShader;

	Direct3DDevice->CreateDomainShader((void*)Code.data(), Code.size(), NULL, &D11Shader);

	Shader->Resource = D11Shader;

	return Shader;
}

FRHIGeometryShader* FD3D11DynamicRHI::RHICreateGeometryShader(const std::vector<uint8>& Code)
{
	//Add Shader Option Later
	ID3D11GeometryShader* D11Shader = nullptr;

	FD3D11GeometryShader* Shader = new FD3D11GeometryShader;

	Direct3DDevice->CreateGeometryShader((void*)Code.data(), Code.size(), NULL, &D11Shader);

	Shader->Resource = D11Shader;

	return Shader;
}

FRHIComputeShader* FD3D11DynamicRHI::RHICreateComputeShader(const std::vector<uint8>& Code)
{
	//Add Shader Option Later
	ID3D11ComputeShader* D11Shader = nullptr;

	FD3D11ComputeShader* Shader = new FD3D11ComputeShader;

	Direct3DDevice->CreateComputeShader((void*)Code.data(), Code.size(), NULL, &D11Shader);

	Shader->Resource = D11Shader;

	return Shader;
}

FRHIPixelShader* FD3D11DynamicRHI::RHICreatePixelShader(const std::vector<uint8>& Code)
{
	//Add Shader Option Later
	ID3D11PixelShader* D11Shader = nullptr;

	FD3D11PixelShader* Shader = new FD3D11PixelShader;

	Direct3DDevice->CreateVertexShader((void*)Code.data(), Code.size(), NULL, &D11Shader);

	Shader->Resource = D11Shader;

	return Shader;
}