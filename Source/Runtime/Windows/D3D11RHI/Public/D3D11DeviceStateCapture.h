#pragma once
#include "D3D11RHI.h"

class FD3D11DeviceStateCapture
{
public:
	FD3D11DeviceStateCapture(FD3D11DeviceContext* InD3D11Context)
		:D3D11Context(InD3D11Context)
	{

	}

	void ReleaseResource();

	void CaptureDeviceState(FD3D11StateCache* InStateCache, TBoundShaderStateHistory<1000>& BoundShaderStateHistory)
	{
		InStateCache->GetConstantBuffer<SF_Vertex>(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, &OldVertexConstantBuffer[0]);
		InStateCache->GetConstantBuffer<SF_Pixel>(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, &OldPixelConstantBuffer[0]);
		InStateCache->GetShaderResourceView<SF_Vertex>(0 , D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT , &OldSRV[0]);
		InStateCache->GetPixelShader(&OldPixelShader);
		InStateCache->GetVertexShader(&OldVertexShader);

		InStateCache->GetInputLayout(&OldInputLayout);

		InStateCache->GetBlendState(&OldBlendState , OldBlendFactor , &OldSampleMask);
		InStateCache->GetDepthStencilState(&OldDepthStencilState , &OldStencilRef);
		InStateCache->GetRasterizerState(&OldRasterizerState);

		OldBoundShaderState = BoundShaderStateHistory.GetLast();
	}

	void RestoreDeviceState(FD3D11StateCache& InStateCache, TBoundShaderStateHistory<10000>& BSSHistory)
	{
		for (int i = 0; i < D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT;i++)
		{
			InStateCache.SetConstantBuffer<SF_Vertex>(OldVertexConstantBuffer[i] , i);
			InStateCache.SetConstantBuffer<SF_Pixel>(OldPixelConstantBuffer[i], i);
		}

		for (int i = 0; i < D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT;i++)
		{
			InStateCache.SetShaderResourceView<SF_Vertex>(OldSRV[i] , i);
		}

		InStateCache.SetPixelShader(OldPixelShader);
		InStateCache.SetVertexShader(OldVertexShader);
		InStateCache.SetInputLayout(OldInputLayout);

		InStateCache.SetBlendState(OldBlendState, OldBlendFactor, OldSampleMask);
		InStateCache.SetDepthStencilState(OldDepthStencilState, OldStencilRef);
		InStateCache.SetRasterizerState(OldRasterizerState);
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
	float OldBlendFactor[4];
	uint32 OldSampleMask;

	ID3D11DepthStencilState* OldDepthStencilState;
	uint32 OldStencilRef;

	ID3D11RasterizerState* OldRasterizerState;

	//SRV
	ID3D11ShaderResourceView* OldSRV[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];

	FRHIBoundShaderState* OldBoundShaderState;
};