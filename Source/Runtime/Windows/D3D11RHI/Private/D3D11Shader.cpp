#include "D3D11RHIPCH.h"
#include "WindowsD3D11DynamicRHI.h"
#include "D3D11Definitions.h"
#include "HAL/Memory/LilithMemory.h"


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

	Direct3DDevice->CreatePixelShader((void*)Code.data(), Code.size(), NULL, &D11Shader);

	Shader->Resource = D11Shader;

	return Shader;
}

FRHIBoundShaderState* FD3D11DynamicRHI::RHICreateBoundShaderState(FRHIVertexDeclaration* InVertexDeclaration, FRHIVertexShader* InVertexShader, FRHIHullShader* InHullShader, FRHIDomainShader* InDomainShader, FRHIGeometryShader* InGeometryShader, FRHIPixelShader* InPixelShader)
{
	//TODO:Wait for BoundStateCache
	FD3D11BoundShaderState* BoundShaderState = new FD3D11BoundShaderState(InVertexDeclaration , InVertexShader , InHullShader , InDomainShader , InGeometryShader , InPixelShader , Direct3DDevice);

	return BoundShaderState;
}

FD3D11BoundShaderState::FD3D11BoundShaderState(FRHIVertexDeclaration* InVertexDeclaration, FRHIVertexShader* InVertexShader, FRHIHullShader* InHullShader, FRHIDomainShader* InDomainShader, FRHIGeometryShader* InGeometryShader, FRHIPixelShader* InPixelShader, ID3D11Device* Direct3DDevice)
	:CacheLink(InVertexDeclaration , InVertexShader , InPixelShader , InHullShader,InDomainShader,InGeometryShader,this)
{
	FD3D11VertexDeclaration* D11VertexDeclaration = (FD3D11VertexDeclaration*)InVertexDeclaration;
	FD3D11VertexShader* D11VertexShader = (FD3D11VertexShader*)InVertexShader;
	FD3D11HullShader* D11HullShader = (FD3D11HullShader*)InHullShader;
	FD3D11DomainShader* D11DomainShader = (FD3D11DomainShader*)InDomainShader;
	FD3D11GeometryShader* D11GeometryShader = (FD3D11GeometryShader*)InGeometryShader;
	FD3D11PixelShader* D11PixelShader = (FD3D11PixelShader*)InPixelShader;


	VertexShader = D11VertexShader->Resource;
	HullShader = D11HullShader->Resource;
	DomainShader = D11DomainShader->Resource;
	GeometryShader = D11GeometryShader->Resource;
	PixelShader = D11PixelShader->Resource;

	Direct3DDevice->CreateInputLayout(D11VertexDeclaration->VertexElements.data(), D11VertexDeclaration->VertexElements.size() , D11VertexShader->Code.data() , D11VertexShader->Code.size(),&InputeLayout);

	FMemory::Memzero(&bShaderNeedsGlobalConstantBuffer, sizeof(bShaderNeedsGlobalConstantBuffer));

	bShaderNeedsGlobalConstantBuffer[SF_Vertex] = D11VertexShader->bShaderNeedsGlobalConstantBuffer;
	bShaderNeedsGlobalConstantBuffer[SF_Hull] = D11HullShader?D11HullShader->bShaderNeedsGlobalConstantBuffer:false;
	bShaderNeedsGlobalConstantBuffer[SF_Domain] = D11DomainShader?D11DomainShader->bShaderNeedsGlobalConstantBuffer:false;
	bShaderNeedsGlobalConstantBuffer[SF_Pixel] = D11GeometryShader ?D11GeometryShader->bShaderNeedsGlobalConstantBuffer:false;
	bShaderNeedsGlobalConstantBuffer[SF_Geometry] = D11PixelShader?D11PixelShader->bShaderNeedsGlobalConstantBuffer:false;

}

void FD3D11DynamicRHI::RHISetComputeShader(FRHIComputeShader* ComputerShader)
{
	FD3D11ComputeShader* D11ComputerShader = (FD3D11ComputeShader*)ComputerShader;

	if (CurrentComputeShader != ComputerShader)
	{
		CurrentComputeShader = D11ComputerShader;
	}
}

void FD3D11DynamicRHI::RHIDispatchComputeShader(uint32 ThreadGroupCountX, uint32 ThreadGroupCountY, uint32 ThreadGroupCountZ)
{
	FD3D11ComputeShader* ComputeShader = CurrentComputeShader;

	StateCache.SetComputeShader(ComputeShader->Resource);
	//commit constants
	CommitComputeShaderConstants();
	//commit resource
	CommitComputeShaderResourceTables(ComputeShader);

	Direct3DDeviceIMContext->Dispatch(ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ);

	StateCache.SetComputeShader((ID3D11ComputeShader*)nullptr);
}

void FD3D11DynamicRHI::RHIDispatchIndirectComputeShader(FRHIVertexBuffer* ArgumentBuffer, uint32 ArgumentOffset)
{
	FD3D11ComputeShader* ComputeShader = CurrentComputeShader;
	FD3D11VertexBuffer* D11VertexBuffer = (FD3D11VertexBuffer*)ArgumentBuffer;

	StateCache.SetComputeShader(ComputeShader->Resource);
	//commit constants
	CommitComputeShaderConstants();
	//commit resource
	CommitComputeShaderResourceTables(ComputeShader);

	Direct3DDeviceIMContext->DispatchIndirect(D11VertexBuffer->Resource , ArgumentOffset);

	StateCache.SetComputeShader((ID3D11ComputeShader*)nullptr);
}

void FD3D11DynamicRHI::RHISetShaderResourceViewParam(FRHIComputeShader* ComputeShader, uint32 SRVIndex, FRHIShaderResourceView* SRV)
{
	FD3D11ShaderResourceView* D3DSRV = (FD3D11ShaderResourceView*)SRV;

	FD3D11ComputeShader* D11ComputeShader = (FD3D11ComputeShader*)ComputeShader;

	SetShaderResourceView<SF_Compute>(D3DSRV->Resource, D3DSRV->View, SRVIndex, "");
}

void FD3D11DynamicRHI::PushEvent(char* Name, FColor Color)
{

}

void FD3D11DynamicRHI::PopEvent()
{

}