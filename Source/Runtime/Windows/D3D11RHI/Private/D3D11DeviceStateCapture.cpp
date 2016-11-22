#include "D3D11RHIPCH.h"
#include "D3D11DeviceStateCapture.h"


void D3D11DeviceStateCapture::ReleaseResource()
{
	SAFE_RELEASE(OldVertexShader);
	SAFE_RELEASE(OldPixelShader);

	SAFE_RELEASE(OldBlendState);
	SAFE_RELEASE(OldDepthStencilState);
	SAFE_RELEASE(OldRasterizerState);

	SAFE_RELEASE(OldInputLayout);

	for (int i = 0; i < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT;i++)
	{
		SAFE_RELEASE(OldVertexConstantBuffer[i]);
		SAFE_RELEASE(OldPixelConstantBuffer[i]);
	}

	for (int i = 0; i < D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT;i++)
	{
		SAFE_RELEASE(OldSRV[i]);
	}

	OldBoundShaderState = NULL;
}