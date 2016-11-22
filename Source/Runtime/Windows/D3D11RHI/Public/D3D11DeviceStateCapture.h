#pragma once
#include "D3D11RHI.h"

class D3D11DeviceStateCapture
{
public:
	D3D11DeviceStateCapture(FD3D11DeviceContext* InD3D11Context)
		:D3D11Context(InD3D11Context)
	{

	}

	void ReleaseResource();

	void CaptureDeviceState(FD3D11StateCache* InStateCache, TBoundShaderStateHistory<1000>& BoundShaderStateHistory)
	{
		InStateCache->get
	}
protected:
private:
	//D3D Context
	FD3D11DeviceContext* D3D11Context;

	//ConstantBuffer
	ID3D11Buffer* OldVertexConstantBuffer[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
	ID3D11Buffer* OldPixelConstantBuffer[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
	//Shader
	ID3D11VertexShader* OldVertexShader;
	ID3D11PixelShader* OldPixelShader;

	//InputeLayout
	ID3D11InputLayout* OldInputLayout;

	//Three State
	ID3D11BlendState* OldBlendState;
	ID3D11DepthStencilState* OldDepthStencilState;
	ID3D11RasterizerState* OldRasterizerState;

	//SRV
	ID3D11ShaderResourceView* OldSRV[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];

	FRHIBoundShaderState* OldBoundShaderState;
};